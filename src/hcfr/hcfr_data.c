#ifndef hcfr_DATA_C
#define hcfr_DATA_C
#include "hcfr_data.h"

const char *hcfr_type[hcfr_type_max] = {
	"REC601",
	"REC601",
	"PALSECAM",
	"PALSECAM",
	"REC709",
	"REC709",
	"SRGB",
	"SRGB"
};
const char *hcfr_wp[hcfr_type_max] = {
	"D65",
	"D93",
	"D65",
	"D93",
	"D65",
	"D93",
	"D65",
	"D93"
};
const char *hcfr_colour_name[hcfr_type_max][hcfr_colour_max] = {
	{ "gray000","gray010","gray020","gray030","gray040","gray050","gray060","gray070","gray080","gray090","gray100","redprimary","greenprimary","blueprimary","white","yellowsecondary","cyansecondary","magentasecondary","white","gray000","gray001","gray002","gray003","gray004","gray096","gray097","gray098","gray099","gray100","red000","red025","red050","red075","red100","green000","green025","green050","green075","green100","blue000","blue025","blue050","blue075","blue100","yellow000","yellow025","yellow050","yellow075","yellow100","cyan000","cyan025","cyan050","cyan075","cyan100","magenta000","magenta025","magenta050","magenta075","magenta100","black","gray35","gray50","gray65","gray80","white","darkskin","lightskin","bluesky","foliage","blueflower","bluishgreen","orange","purplishblue","moderatered","purple","yellowgreen","orangeyellow","blue","green","red","yellow","magenta","cyan"	},
	{ "gray000","gray010","gray020","gray030","gray040","gray050","gray060","gray070","gray080","gray090","gray100","redprimary","greenprimary","blueprimary","white","yellowsecondary","cyansecondary","magentasecondary","white","gray000","gray001","gray002","gray003","gray004","gray096","gray097","gray098","gray099","gray100","red000","red025","red050","red075","red100","green000","green025","green050","green075","green100","blue000","blue025","blue050","blue075","blue100","yellow000","yellow025","yellow050","yellow075","yellow100","cyan000","cyan025","cyan050","cyan075","cyan100","magenta000","magenta025","magenta050","magenta075","magenta100","black","gray35","gray50","gray65","gray80","white","darkskin","lightskin","bluesky","foliage","blueflower","bluishgreen","orange","purplishblue","moderatered","purple","yellowgreen","orangeyellow","blue","green","red","yellow","magenta","cyan"	},
	{ "gray000","gray010","gray020","gray030","gray040","gray050","gray060","gray070","gray080","gray090","gray100","redprimary","greenprimary","blueprimary","white","yellowsecondary","cyansecondary","magentasecondary","white","gray000","gray001","gray002","gray003","gray004","gray096","gray097","gray098","gray099","gray100","red000","red025","red050","red075","red100","green000","green025","green050","green075","green100","blue000","blue025","blue050","blue075","blue100","yellow000","yellow025","yellow050","yellow075","yellow100","cyan000","cyan025","cyan050","cyan075","cyan100","magenta000","magenta025","magenta050","magenta075","magenta100","black","gray35","gray50","gray65","gray80","white","darkskin","lightskin","bluesky","foliage","blueflower","bluishgreen","orange","purplishblue","moderatered","purple","yellowgreen","orangeyellow","blue","green","red","yellow","magenta","cyan"	},
	{ "gray000","gray010","gray020","gray030","gray040","gray050","gray060","gray070","gray080","gray090","gray100","redprimary","greenprimary","blueprimary","white","yellowsecondary","cyansecondary","magentasecondary","white","gray000","gray001","gray002","gray003","gray004","gray096","gray097","gray098","gray099","gray100","red000","red025","red050","red075","red100","green000","green025","green050","green075","green100","blue000","blue025","blue050","blue075","blue100","yellow000","yellow025","yellow050","yellow075","yellow100","cyan000","cyan025","cyan050","cyan075","cyan100","magenta000","magenta025","magenta050","magenta075","magenta100","black","gray35","gray50","gray65","gray80","white","darkskin","lightskin","bluesky","foliage","blueflower","bluishgreen","orange","purplishblue","moderatered","purple","yellowgreen","orangeyellow","blue","green","red","yellow","magenta","cyan"	},
	{ "gray000","gray010","gray020","gray030","gray040","gray050","gray060","gray070","gray080","gray090","gray100","redprimary","greenprimary","blueprimary","white","yellowsecondary","cyansecondary","magentasecondary","white","gray000","gray001","gray002","gray003","gray004","gray096","gray097","gray098","gray099","gray100","red000","red025","red050","red075","red100","green000","green025","green050","green075","green100","blue000","blue025","blue050","blue075","blue100","yellow000","yellow025","yellow050","yellow075","yellow100","cyan000","cyan025","cyan050","cyan075","cyan100","magenta000","magenta025","magenta050","magenta075","magenta100","black","gray35","gray50","gray65","gray80","white","darkskin","lightskin","bluesky","foliage","blueflower","bluishgreen","orange","purplishblue","moderatered","purple","yellowgreen","orangeyellow","blue","green","red","yellow","magenta","cyan"	},
	{ "gray000","gray010","gray020","gray030","gray040","gray050","gray060","gray070","gray080","gray090","gray100","redprimary","greenprimary","blueprimary","white","yellowsecondary","cyansecondary","magentasecondary","white","gray000","gray001","gray002","gray003","gray004","gray096","gray097","gray098","gray099","gray100","red000","red025","red050","red075","red100","green000","green025","green050","green075","green100","blue000","blue025","blue050","blue075","blue100","yellow000","yellow025","yellow050","yellow075","yellow100","cyan000","cyan025","cyan050","cyan075","cyan100","magenta000","magenta025","magenta050","magenta075","magenta100","black","gray35","gray50","gray65","gray80","white","darkskin","lightskin","bluesky","foliage","blueflower","bluishgreen","orange","purplishblue","moderatered","purple","yellowgreen","orangeyellow","blue","green","red","yellow","magenta","cyan"	},
	{ "gray000","gray010","gray020","gray030","gray040","gray050","gray060","gray070","gray080","gray090","gray100","redprimary","greenprimary","blueprimary","white","yellowsecondary","cyansecondary","magentasecondary","white","gray000","gray001","gray002","gray003","gray004","gray096","gray097","gray098","gray099","gray100","red000","red025","red050","red075","red100","green000","green025","green050","green075","green100","blue000","blue025","blue050","blue075","blue100","yellow000","yellow025","yellow050","yellow075","yellow100","cyan000","cyan025","cyan050","cyan075","cyan100","magenta000","magenta025","magenta050","magenta075","magenta100","black","gray35","gray50","gray65","gray80","white","darkskin","lightskin","bluesky","foliage","blueflower","bluishgreen","orange","purplishblue","moderatered","purple","yellowgreen","orangeyellow","blue","green","red","yellow","magenta","cyan"	},
	{ "gray000","gray010","gray020","gray030","gray040","gray050","gray060","gray070","gray080","gray090","gray100","redprimary","greenprimary","blueprimary","white","yellowsecondary","cyansecondary","magentasecondary","white","gray000","gray001","gray002","gray003","gray004","gray096","gray097","gray098","gray099","gray100","red000","red025","red050","red075","red100","green000","green025","green050","green075","green100","blue000","blue025","blue050","blue075","blue100","yellow000","yellow025","yellow050","yellow075","yellow100","cyan000","cyan025","cyan050","cyan075","cyan100","magenta000","magenta025","magenta050","magenta075","magenta100","black","gray35","gray50","gray65","gray80","white","darkskin","lightskin","bluesky","foliage","blueflower","bluishgreen","orange","purplishblue","moderatered","purple","yellowgreen","orangeyellow","blue","green","red","yellow","magenta","cyan"	}
};
unsigned char hcfr_colour_r[hcfr_type_max][hcfr_colour_max] = {
	{ 0,26,51,77,102,128,153,178,204,229,255,255,0,0,255,255,0,255,255,0,2,5,8,10,245,247,250,253,255,127,171,205,232,255,218,176,136,94,0,85,83,78,68,0,245,248,252,253,255,229,201,168,123,0,148,168,191,219,255,0,158,186,210,229,255,115,193,94,90,130,99,217,75,193,92,158,229,51,71,176,238,186,0	},
	{ 0,26,51,77,102,128,153,178,204,229,255,255,0,0,255,255,0,255,255,0,2,5,8,10,245,247,250,253,255,120,171,206,233,255,217,171,132,90,0,98,94,88,77,0,241,246,250,253,255,232,205,171,126,0,149,169,191,219,255,0,158,186,210,229,255,115,193,94,90,130,99,217,75,193,92,158,229,51,71,176,238,186,0	},
	{ 0,26,51,77,102,128,153,178,204,229,255,255,0,0,255,255,0,255,255,0,2,5,8,10,245,247,250,253,255,129,172,204,232,255,218,176,136,94,0,78,76,72,64,0,247,249,252,254,255,228,200,167,122,0,147,167,190,219,255,0,158,186,210,229,255,115,193,94,90,130,99,217,75,193,92,158,229,51,71,176,238,186,0	},
	{ 0,26,51,77,102,128,153,178,204,229,255,255,0,0,255,255,0,255,255,0,2,5,8,10,245,247,250,253,255,123,172,206,233,255,218,172,132,90,0,90,87,83,72,0,243,248,250,253,255,231,204,170,125,0,148,167,190,218,255,0,158,186,210,229,255,115,193,94,90,130,99,217,75,193,92,158,229,51,71,176,238,186,0	},
	{ 0,26,51,77,102,128,153,178,204,229,255,255,0,0,255,255,0,255,255,0,2,5,8,10,245,247,250,253,255,127,171,204,231,255,219,177,137,94,0,78,76,72,64,0,247,249,252,254,255,229,201,168,122,0,144,165,189,218,255,0,158,186,210,229,255,115,193,94,90,130,99,217,75,193,92,158,229,51,71,176,238,186,0	},
	{ 0,26,51,77,102,128,153,178,204,229,255,255,0,0,255,255,0,255,255,0,2,5,8,10,245,247,250,253,255,121,171,205,233,255,219,173,133,90,0,90,87,83,72,0,243,248,250,253,255,232,204,171,126,0,146,165,187,217,255,0,158,186,210,229,255,115,193,94,90,130,99,217,75,193,92,158,229,51,71,176,238,186,0	},
	{ 0,29,54,79,105,130,154,179,204,230,255,255,0,0,255,255,0,255,255,0,3,7,11,14,245,247,250,253,255,129,172,204,231,255,219,178,139,97,0,80,78,75,67,0,247,249,252,254,255,230,202,169,124,0,146,167,190,218,255,0,160,188,211,230,255,117,195,95,91,132,100,218,75,195,93,160,230,50,71,178,238,188,0	},
	{ 0,29,54,79,105,130,154,179,204,230,255,255,0,0,255,255,0,255,255,0,3,7,11,14,245,247,250,253,255,123,172,206,233,255,219,175,134,92,0,92,90,85,75,0,244,248,250,253,255,232,204,172,128,0,147,167,188,217,255,0,160,188,211,230,255,117,195,95,91,132,100,218,75,195,93,160,230,50,71,178,238,188,0	}
};
unsigned char hcfr_colour_g[hcfr_type_max][hcfr_colour_max] = {
	{ 0,26,51,77,102,128,153,178,204,229,255,0,255,0,255,255,255,0,255,0,2,5,8,10,245,247,250,253,255,127,111,92,68,0,218,233,242,249,255,85,83,78,68,0,245,248,252,253,255,229,235,242,249,255,148,139,123,99,0,0,158,186,210,229,255,82,150,122,107,128,189,120,92,84,62,186,161,62,148,49,199,84,133	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,255,0,255,255,255,0,255,0,2,5,8,10,245,247,250,253,255,120,104,85,61,0,217,233,243,250,255,98,94,88,77,0,241,246,250,253,255,232,238,243,249,255,149,140,126,101,0,0,158,186,210,229,255,82,150,122,107,128,189,120,92,84,62,186,161,62,148,49,199,84,133	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,255,0,255,255,255,0,255,0,2,5,8,10,245,247,250,253,255,129,114,94,69,0,218,233,242,249,255,78,76,72,64,0,247,249,252,254,255,228,235,242,248,255,147,137,123,98,0,0,158,186,210,229,255,82,150,122,107,128,189,120,92,84,62,186,161,62,148,49,199,84,133	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,255,0,255,255,255,0,255,0,2,5,8,10,245,247,250,253,255,123,107,87,63,0,218,243,243,250,255,90,87,83,72,0,243,248,250,253,255,231,236,243,249,255,148,139,125,100,0,0,158,186,210,229,255,82,150,122,107,128,189,120,92,84,62,186,161,62,148,49,199,84,133	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,255,0,255,255,255,0,255,0,2,5,8,10,245,247,250,253,255,127,112,93,68,0,219,234,243,250,255,78,76,72,64,0,247,249,252,254,255,229,236,242,249,255,144,136,122,98,0,0,158,186,210,229,255,82,150,122,107,128,189,120,92,84,62,186,161,62,148,49,199,84,133	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,255,0,255,255,255,0,255,0,2,5,8,10,245,247,250,253,255,121,105,86,62,0,219,234,243,250,255,90,87,83,72,0,243,248,250,253,255,232,238,243,249,255,146,137,123,100,0,0,158,186,210,229,255,82,150,122,107,128,189,120,92,84,62,186,161,62,148,49,199,84,133	},
	{ 0,29,54,79,105,130,154,179,204,230,255,0,255,0,255,255,255,0,255,0,3,7,11,14,245,247,250,253,255,129,114,95,70,0,219,234,244,250,255,80,78,75,67,0,247,249,252,254,255,230,237,242,249,255,146,138,124,100,0,0,160,188,211,230,255,82,153,124,109,130,190,122,93,84,61,188,163,61,150,47,201,84,135	},
	{ 0,29,54,79,105,130,154,179,204,230,255,0,255,0,255,255,255,0,255,0,3,7,11,14,245,247,250,253,255,123,107,88,64,0,219,234,244,250,255,92,90,85,75,0,244,248,250,253,255,232,238,244,249,255,147,139,125,102,0,0,160,188,211,230,255,82,152,124,109,130,190,122,93,84,61,188,163,61,150,47,201,84,135	}
};
unsigned char hcfr_colour_b[hcfr_type_max][hcfr_colour_max] = {
	{ 0,26,51,77,102,128,153,178,204,229,255,0,0,255,255,0,255,255,255,0,2,5,8,10,245,247,250,253,255,127,111,92,68,0,218,176,136,94,0,85,107,136,177,255,245,203,161,113,0,229,235,242,249,255,148,168,191,219,255,0,158,186,210,229,255,66,130,156,66,176,169,41,163,97,107,64,45,150,71,58,33,146,163	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,0,255,255,0,255,255,255,0,2,5,8,10,245,247,250,253,255,120,104,85,61,0,217,171,132,90,0,98,119,146,184,255,241,196,153,106,0,232,238,243,249,255,149,169,191,219,255,0,158,186,210,229,255,66,130,156,66,176,169,41,163,97,107,64,45,150,71,58,33,146,163	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,0,255,255,0,255,255,255,0,2,5,8,10,245,247,250,253,255,129,114,94,69,0,218,176,136,94,0,78,100,127,169,255,247,205,163,114,0,228,235,242,248,255,147,167,190,219,255,0,158,186,210,229,255,66,130,156,66,176,169,41,163,97,107,64,45,150,71,58,33,146,163	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,0,255,255,0,255,255,255,0,2,5,8,10,245,247,250,253,255,123,107,87,63,0,218,172,132,90,0,90,111,137,176,255,243,198,155,107,0,231,236,243,249,255,148,167,190,218,255,0,158,186,210,229,255,66,130,156,66,176,169,41,163,97,107,64,45,150,71,58,33,146,163	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,0,255,255,0,255,255,255,0,2,5,8,10,245,247,250,253,255,127,112,93,68,0,219,177,137,94,0,78,100,128,169,255,247,205,162,113,0,229,236,242,249,255,144,165,189,218,255,0,158,186,210,229,255,66,130,156,66,176,169,41,163,97,107,64,45,150,71,58,33,146,163	},
	{ 0,26,51,77,102,128,153,178,204,229,255,0,0,255,255,0,255,255,255,0,2,5,8,10,245,247,250,253,255,121,105,86,62,0,219,173,133,90,0,90,111,137,177,255,243,198,155,107,0,232,238,243,249,255,146,165,187,217,255,0,158,186,210,229,255,66,130,156,66,176,169,41,163,97,107,64,45,150,71,58,33,146,163	},
	{ 0,29,54,79,105,130,154,179,204,230,255,0,0,255,255,0,255,255,255,0,3,7,11,14,245,247,250,253,255,129,114,95,70,0,219,178,139,97,0,80,102,130,170,255,247,206,163,115,0,230,237,242,249,255,146,167,190,218,255,0,160,188,211,230,255,66,132,158,66,178,171,38,165,98,109,64,43,152,71,57,29,148,165	},
	{ 0,29,54,79,105,130,154,179,204,230,255,0,0,255,255,0,255,255,255,0,3,7,11,14,245,247,250,253,255,123,107,88,64,0,219,175,134,92,0,92,113,139,178,255,244,199,156,109,0,232,238,244,249,255,147,167,188,217,255,0,160,188,211,230,255,66,132,158,66,178,171,38,165,98,109,64,43,152,71,57,29,148,165	}
};

#endif
