#include <verilated.h>
#include "Vemu.h"

#include "imgui.h"
#ifndef _MSC_VER
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#else
#define WIN32
#include <dinput.h>
#endif

#include "sim_console.h"
#include "sim_bus.h"
#include "sim_video.h"
#include "sim_input.h"
#include "sim_clock.h"

#include "../imgui/imgui_memory_editor.h"
#include "../imgui/ImGuiFileDialog.h"

#include <iostream>
#include <fstream>
using namespace std;

// Simulation control
// ------------------
int initialReset = 48;
bool run_enable = 1;
int batchSize = 150000;
bool single_step = 0;
bool multi_step = 0;
int multi_step_amount = 1024;

// Debug GUI 
// ---------
const char* windowTitle = "Verilator Sim: Aznable";
const char* windowTitle_Control = "Simulation control";
const char* windowTitle_DebugLog = "Debug log";
const char* windowTitle_Video = "VGA output";
bool showDebugLog = true;
DebugConsole console;
MemoryEditor mem_edit_1;
MemoryEditor mem_edit_2;
MemoryEditor mem_edit_3;
MemoryEditor mem_edit_4;
// HPS emulator
// ------------
SimBus bus(console);

// Input handling
// --------------
SimInput input(12, console);
const int input_right = 0;
const int input_left = 1;
const int input_down = 2;
const int input_up = 3;
const int input_a = 4;
const int input_b = 5;
const int input_x = 6;
const int input_y = 7;
const int input_l = 8;
const int input_r = 9;
const int input_select = 10;
const int input_start = 11;

// Video
// -----
#define VGA_WIDTH 320
#define VGA_HEIGHT 240
#define VGA_ROTATE 0  // 90 degrees anti-clockwise
#define VGA_SCALE_X vga_scale
#define VGA_SCALE_Y vga_scale
SimVideo video(VGA_WIDTH, VGA_HEIGHT, VGA_ROTATE);
float vga_scale = 2.0;


// Verilog module
// --------------
Vemu* top = NULL;

vluint64_t main_time = 0;	// Current simulation time.
double sc_time_stamp() {	// Called by $time in Verilog.
	return main_time;
}

SimClock clk_sys(1);

#define DEBUG_AUDIO

#ifdef DEBUG_AUDIO
// Audio
SimClock clk_audio(1000);
ofstream audioFile;
#endif


// Reset simulation variables and clocks
void resetSim() {
	main_time = 0;
	top->reset = 1;
	clk_sys.Reset();
#ifdef DEBUG_AUDIO
	clk_audio.Reset();
#endif
}

int verilate() {

	if (!Verilated::gotFinish()) {

		// Assert reset during startup
		if (main_time < initialReset) { top->reset = 1; }
		// Deassert reset after startup
		if (main_time == initialReset) { top->reset = 0; }

		// Clock dividers
		clk_sys.Tick();

		// Set system clock in core
		top->clk_sys = clk_sys.clk;

		// Simulate both edges of system clock
		if (clk_sys.clk != clk_sys.old) {
			if (clk_sys.clk) {
				input.BeforeEval();
				bus.BeforeEval();
			}
			top->eval();
			if (clk_sys.clk) { bus.AfterEval(); }
		}

#ifdef DEBUG_AUDIO
		clk_audio.Tick();
		if (clk_audio.IsRising()) {
			// Output audio
			unsigned short audio_l = top->AUDIO_L;
			//unsigned short audio_r = top->AUDIO_R;
			audioFile.write((const char*)&audio_l, 2);
			//	audioFile.write((const char*)&audio_r, 2);
		}
#endif

		// Output pixels on rising edge of pixel clock
		if (clk_sys.IsRising() && top->emu__DOT__ce_6) {
			uint32_t colour = 0xFF000000 | top->VGA_B << 16 | top->VGA_G << 8 | top->VGA_R;
			video.Clock(top->VGA_HB, top->VGA_VB, top->VGA_HS, top->VGA_VS, colour);
		}

		main_time++;
		return 1;
	}

	// Stop verilating and cleanup
	top->final();
	delete top;
	exit(0);
	return 0;
}

unsigned char mouse_clock = 0;
unsigned char mouse_buttons = 0;
unsigned char mouse_x = 0;
unsigned char mouse_y = 0;

char spinner_toggle = 0;

int main(int argc, char** argv, char** env) {

	// Create core and initialise
	top = new Vemu();
	Verilated::commandArgs(argc, argv);

#ifdef WIN32
	// Attach debug console to the verilated code
	Verilated::setDebug(console);
#endif

	// Attach bus
	bus.ioctl_addr = &top->ioctl_addr;
	bus.ioctl_index = &top->ioctl_index;
	bus.ioctl_wait = &top->ioctl_wait;
	bus.ioctl_download = &top->ioctl_download;
	//bus.ioctl_upload = &top->ioctl_upload;
	bus.ioctl_wr = &top->ioctl_wr;
	bus.ioctl_dout = &top->ioctl_dout;
	//bus.ioctl_din = &top->ioctl_din;
	input.ps2_key = &top->ps2_key;

	// Setup Audio output stream
	audioFile.open("audio.wav", ios::binary);

	// Set up input module
	input.Initialise();
#ifdef WIN32
	input.SetMapping(input_up, DIK_UP);
	input.SetMapping(input_right, DIK_RIGHT);
	input.SetMapping(input_down, DIK_DOWN);
	input.SetMapping(input_left, DIK_LEFT);
	input.SetMapping(input_a, DIK_Z); // A
	input.SetMapping(input_b, DIK_X); // B
	input.SetMapping(input_x, DIK_A); // X
	input.SetMapping(input_y, DIK_S); // Y
	input.SetMapping(input_l, DIK_Q); // L
	input.SetMapping(input_r, DIK_W); // R
	input.SetMapping(input_select, DIK_1); // Select
	input.SetMapping(input_start, DIK_2); // Start

#else
	input.SetMapping(input_up, SDL_SCANCODE_UP);
	input.SetMapping(input_right, SDL_SCANCODE_RIGHT);
	input.SetMapping(input_down, SDL_SCANCODE_DOWN);
	input.SetMapping(input_left, SDL_SCANCODE_LEFT);
	input.SetMapping(input_fire1, SDL_SCANCODE_SPACE);
	input.SetMapping(input_start_1, SDL_SCANCODE_1);
	input.SetMapping(input_start_2, SDL_SCANCODE_2);
	input.SetMapping(input_coin_1, SDL_SCANCODE_3);
	input.SetMapping(input_coin_2, SDL_SCANCODE_4);
	input.SetMapping(input_coin_3, SDL_SCANCODE_5);
	input.SetMapping(input_pause, SDL_SCANCODE_P);
#endif
	// Setup video output
	if (video.Initialise(windowTitle) == 1) { return 1; }

#ifdef WIN32
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
#else
	bool done = false;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
		}
#endif
		video.StartFrame();

		input.Read();


		// Draw GUI
		// --------
		ImGui::NewFrame();

		// Simulation control window
		ImGui::Begin(windowTitle_Control);
		ImGui::SetWindowPos(windowTitle_Control, ImVec2(0, 0), ImGuiCond_Once);
		ImGui::SetWindowSize(windowTitle_Control, ImVec2(500, 150), ImGuiCond_Once);
		if (ImGui::Button("Reset simulation")) { resetSim(); } ImGui::SameLine();
		if (ImGui::Button("Start running")) { run_enable = 1; } ImGui::SameLine();
		if (ImGui::Button("Stop running")) { run_enable = 0; } ImGui::SameLine();
		ImGui::Checkbox("RUN", &run_enable);
		//ImGui::PopItemWidth();
		ImGui::SliderInt("Run batch size", &batchSize, 1, 250000);
		if (single_step == 1) { single_step = 0; }
		if (ImGui::Button("Single Step")) { run_enable = 0; single_step = 1; }
		ImGui::SameLine();
		if (multi_step == 1) { multi_step = 0; }
		if (ImGui::Button("Multi Step")) { run_enable = 0; multi_step = 1; }
		//ImGui::SameLine();
		ImGui::SliderInt("Multi step amount", &multi_step_amount, 8, 1024);

		ImGui::End();

		// Debug log window
		console.Draw(windowTitle_DebugLog, &showDebugLog, ImVec2(500, 700));
		ImGui::SetWindowPos(windowTitle_DebugLog, ImVec2(0, 160), ImGuiCond_Once);

		// Memory debug
		/*ImGui::Begin("PGROM Editor");
		mem_edit_1.DrawContents(top->emu__DOT__system__DOT__pgrom__DOT__mem, 16384, 0);
		ImGui::End();
		ImGui::Begin("CHROM Editor");
		mem_edit_1.DrawContents(top->emu__DOT__system__DOT__chrom__DOT__mem, 2048, 0);
		ImGui::End();*/
		/*ImGui::Begin("WKRAM Editor");
		mem_edit_2.DrawContents(&top->emu__DOT__system__DOT__wkram__DOT__mem, 16384, 0);
		ImGui::End();
		*///ImGui::Begin("CHRAM Editor");
		//mem_edit_3.DrawContents(&top->emu__DOT__system__DOT__chram__DOT__mem, 2048, 0);
		//ImGui::End();
		//ImGui::Begin("FGCOLRAM Editor");
		//mem_edit_3.DrawContents(&top->emu__DOT__system__DOT__fgcolram__DOT__mem, 2048, 0);
		//ImGui::End();
		//ImGui::Begin("BGCOLRAM Editor");
		//mem_edit_1.DrawContents(&top->emu__DOT__system__DOT__bgcolram__DOT__mem, 2048, 0);
		//ImGui::End();
		ImGui::Begin("Sprite RAM");
		mem_edit_1.DrawContents(&top->emu__DOT__system__DOT__spriteram__DOT__mem, 96, 0);
		ImGui::End();

		//ImGui::Begin("Sprite Linebuffer RAM");
		//mem_edit_2.DrawContents(&top->emu__DOT__system__DOT__spritelbram__DOT__mem, 1024, 0);
		//ImGui::End();

		//ImGui::Begin("Sprite Collision Buffer RAM A");
		//mem_edit_2.DrawContents(&top->emu__DOT__system__DOT__comet__DOT__spritecollisionbufferram_a__DOT__mem, 512, 0);
		//ImGui::End();
		//ImGui::Begin("Sprite Collision Buffer RAM B");
		//mem_edit_2.DrawContents(&top->emu__DOT__system__DOT__comet__DOT__spritecollisionbufferram_b__DOT__mem, 512, 0);
		//ImGui::End();
		ImGui::Begin("Sprite Collision RAM ");
		mem_edit_2.DrawContents(&top->emu__DOT__system__DOT__spritecollisionram__DOT__mem, 32, 0);
		ImGui::End();
		//ImGui::Begin("Sprite Debug RAM");
		//mem_edit_2.DrawContents(&top->emu__DOT__system__DOT__spritedebugram__DOT__mem, 128000, 0);
		//ImGui::End();
		/*ImGui::Begin("Palette ROM");
		mem_edit_3.DrawContents(&top->emu__DOT__system__DOT__palrom__DOT__mem, 64, 0);
		ImGui::End();
		ImGui::Begin("Sprite ROM");---
		mem_edit_4.DrawContents(&top->emu__DOT__system__DOT__spriterom__DOT__mem, 2048, 0);
		ImGui::End();*/

		// Video window
		ImGui::Begin(windowTitle_Video);
		ImGui::SetWindowPos(windowTitle_Video, ImVec2(550, 0), ImGuiCond_Once);
		ImGui::SetWindowSize(windowTitle_Video, ImVec2((VGA_WIDTH * VGA_SCALE_X) + 24, (VGA_HEIGHT * VGA_SCALE_Y) + 114), ImGuiCond_Once);

		ImGui::SliderFloat("Zoom", &vga_scale, 1, 8);
		ImGui::SliderInt("Rotate", &video.output_rotate, -1, 1); ImGui::SameLine();
		ImGui::Checkbox("Flip V", &video.output_vflip);
		ImGui::Text("main_time: %d frame_count: %d sim FPS: %f", main_time, video.count_frame, video.stats_fps);
		ImGui::Text("pixel: %06d line: %03d", video.count_pixel, video.count_line);

		float vol_l = ((top->AUDIO_L) / 256.0f) / 256.0f;
		float vol_r = ((top->AUDIO_R) / 256.0f) / 256.0f;
		ImGui::ProgressBar(vol_l, ImVec2(200, 16), 0); ImGui::SameLine();
		ImGui::ProgressBar(vol_r, ImVec2(200, 16), 0);

		// Draw VGA output
		ImGui::Image(video.texture_id, ImVec2(video.output_width * VGA_SCALE_X, video.output_height * VGA_SCALE_Y));
		ImGui::End();

		video.UpdateTexture();

		// Pass inputs to sim
		top->joystick_0 = 0;
		for (int i = 0; i < input.inputCount; i++)
		{
			if (input.inputs[i]) { top->joystick_0 |= (1 << i); }
		}
		top->joystick_1 = top->joystick_0;

		/*top->joystick_analog_0 += 1;
		top->joystick_analog_0 -= 256;*/
		//top->paddle_0 += 1;
		//if (input.inputs[0] || input.inputs[1]) {
		//	spinner_toggle = !spinner_toggle;
		//	top->spinner_0 = (input.inputs[0]) ? 16 : -16;
		//	for (char b = 8; b < 16; b++) {
		//		top->spinner_0 &= ~(1UL << b);
		//	}
		//	if (spinner_toggle) { top->spinner_0 |= 1UL << 8; }
		//}
		/*mouse_buttons += 1;
		mouse_x += 1;
		mouse_y -= 1;
		unsigned long mouse_temp = mouse_buttons;
		mouse_temp += (mouse_x << 8);
		mouse_temp += (mouse_y << 16);
		if (mouse_clock) { mouse_temp |= (1UL << 24); }
		mouse_clock = !mouse_clock;*/
		//top->ps2_mouse = mouse_temp;
		//top->ps2_mouse_ext = mouse_x + (mouse_buttons << 8);

		// Run simulation
		if (run_enable) {
			for (int step = 0; step < batchSize; step++) { verilate(); }
		}
		else {
			if (single_step) { verilate(); }
			if (multi_step) {
				for (int step = 0; step < multi_step_amount; step++) { verilate(); }
			}
		}
	}

	// Clean up before exit
	// --------------------

#ifdef DEBUG_AUDIO
	audioFile.close();
#endif

	video.CleanUp();
	input.CleanUp();

	return 0;
}
