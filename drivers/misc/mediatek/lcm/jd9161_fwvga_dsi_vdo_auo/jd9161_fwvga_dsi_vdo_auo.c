#ifdef BUILD_LK
#include <string.h>
#include <mt_gpio.h>
#include <platform/mt_pmic.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <linux/string.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#endif

#include "lcm_drv.h"

#define FRAME_WIDTH                                         (480)
#define FRAME_HEIGHT                                        (854)
#define LCM_ID						 (0x9161)

#define REGFLAG_DELAY                                       0xFD
#define REGFLAG_END_OF_TABLE                                0x00   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE                                    0

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

//static kal_bool IsFirstBoot = KAL_TRUE;

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)            lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)                                           lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)                   lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)   

#define   LCM_DSI_CMD_MODE                          0

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
	

//Initial setting for C0508(480x854 IPS Column) Panel
//Resolution: 480x854
//External system porch setting: VS=4, VBP=6, VFP=6, HS=10, HBP=10, HFP=10
//Frame rate: 60HZ
//MIPI CLK:300Mbps
//VCI=2.8V, IOVCC=1.8V
//-----initial IV4.5_IPS_Gamma2.2 480x854 DSI VDO -----//


{0xBF,3,{0x91,0x61,0xF2}}, //BF 03 91 61 F2

//VCOM
{0xB3,2,{0x00,0x76}}, //B3 02 00 76

//VCOM_R
{0xB4,2,{0x00,0x76}}, //B4 02 00 76

//VGMP, VGSP, VGMN, VGSN
{0xB8,6,{0x00,0xBF,0x01,0x00,0xBF,0x01}},  //VGSN[7:0]	//B8 06 00 BF 01 00 BF 01						

//GIP output voltage level
{0xBA,3,{0x34,0x23,0x00}}, //BA 03 34 23 00

{0xC0,1,{0x2A}}, //C0 01 2A

{0xC1,1,{0x01}}, //C1 01 01

//column
{0xC3,1,{0x04}},  //C3 01 04

//TCON
{0xC4,2,{0x30,0x6A}},  //854 LINE //C4 02 30 6A

//POWER CTRL
{0xC7,9,{0x00,0x01,0x31,0x05,0x65,0x2A,0x13,0xA5,0xA5}}, //C7 09 00 01 31 05 65 2A 13 A5 A5

//Gamma
//C8 26 7F 51 3D 2E 27 18 1C 05 1E 1D 1D 3B 2B 35 29 27 1D 0F 02 7F 51 3D 2E 27 18 1C 05 1E 1D 1D 3B 2B 35 29 27 1D 0F 02
{0xC8,38,{0x7F,0x51,0x3D,0x2E,0x27,0x18,0x1C,0x05,0x1E,0x1D,0x1D,0x3B,0x2B,0x35,0x29,0x27,0x1D,0x0F,0x02,
          0x7F,0x51,0x3D,0x2E,0x27,0x18,0x1C,0x05,0x1E,0x1D,0x1D,0x3B,0x2B,0x35,0x29,0x27,0x1D,0x0F,0x02}}, 

//SET GIP_L
//D4 10 1F 1E 1F 00 10 1F 1F 04 06 08 0A 1F 1F 1F 1F 1F
{0xD4,16,{0x1F,0x1E,0x1F,0x00,0x10,0x1F,0x1F,0x04,0x06,0x08,0x0A,0x1F,0x1F,0x1F,0x1F,0x1F}},

//SET GIP_R
//D5 10 1F 1E 1F 01 11 1F 1F 05 07 09 0B 1F 1F 1F 1F 1F
{0xD5,16,{0x1F,0x1E,0x1F,0x01,0x11,0x1F,0x1F,0x05,0x07,0x09,0x0B,0x1F,0x1F,0x1F,0x1F,0x1F}},

//SET GIP_GS_L
//D6 10 1F 1F 1E 11 01 1F 1F 09 05 07 0B 1F 1F 1F 1F 1F
{0xD6,16,{0x1F,0x1F,0x1E,0x11,0x01,0x1F,0x1F,0x09,0x05,0x07,0x0B,0x1F,0x1F,0x1F,0x1F,0x1F}},

//SET GIP_GS_R
//D7 10 1F 1F 1E 10 00 1F 1F 08 04 06 0A 1F 1F 1F 1F 1F
{0xD7,16,{0x1F,0x1F,0x1E,0x10,0x00,0x1F,0x1F,0x08,0x04,0x06,0x0A,0x1F,0x1F,0x1F,0x1F,0x1F}},

//SET GIP1
//D8 14 20 02 0A 10 05 30 01 02 30 01 02 06 70 53 61 73 09 06 70 08
{0xD8,20,{0x20,0x02,0x0A,0x10,0x05,0x30,0x01,0x02,0x30,0x01,0x02,0x06,0x70,0x53,0x61,0x73,0x09,0x06,0x70,0x08}},

//SET GIP2
//D9 13 00 0A 0A 88 00 00 06 7B 00 00 00 3B 2F 1F 00 00 00 03 7B
{0xD9,19,{0x00,0x0A,0x0A,0x88,0x00,0x00,0x06,0x7B,0x00,0x00,0x00,0x3B,0x2F,0x1F,0x00,0x00,0x00,0x03,0x7B}},

{0xBE,1,{0x01}}, //BE 01 01

{0xCC,10,{0x34,0x20,0x38,0x60,0x11,0x91,0x00,0x40,0x00,0x00}}, //CC 0A 34 20 38 60 11 91 00 40 00 00

{0xBE,1,{0x00}}, //BE 01 00

{0x11,0,{0x00}}, //11
{REGFLAG_DELAY,120,{}}, //FD 78
{0x29,0,{0x00}}, //29
{REGFLAG_DELAY,30,{}}, //FD 1E

{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_compare_id_setting[] = {
     // Display off sequence
    {0xBF,  3,  {0x91, 0x61, 0xF2}},
    {REGFLAG_DELAY, 10, {}},


    {REGFLAG_END_OF_TABLE, 0x00, {}}
};
static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
    {0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
    {0x29, 0, {0x00}},
    {REGFLAG_DELAY, 10, {}},
    
    //{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {
    // Display off sequence
    {0x28, 0, {0x00}},
    {REGFLAG_DELAY, 10, {}},

    // Sleep Mode On
	{0x10, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	//{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++) {
        
        unsigned cmd;
        cmd = table[i].cmd;
        
        switch (cmd) {
            
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
                
            //case REGFLAG_END_OF_TABLE :
            //    break;
                
            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
                MDELAY(2);
        }
    }
    
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

		// enable tearing-free
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;
#endif
	
		// DSI
		/* Command mode setting */
		params->dsi.LANE_NUM				= LCM_TWO_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

		params->dsi.vertical_sync_active				= 4;// 4;2
		params->dsi.vertical_backporch					= 6;// 8;2
		params->dsi.vertical_frontporch					= 50;// 8;2
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 10;//6
		params->dsi.horizontal_backporch				= 80;//37
		params->dsi.horizontal_frontporch				= 80;//37
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

#if 0    		
		params->dsi.pll_div1=0;//1;		// div1=0,1,2,3;div1_real=1,2,4,4
		params->dsi.pll_div2=2;//1;		// div2=0,1,2,3;div2_real=1,2,4,4
		//params->dsi.fbk_sel=1;		 // fbk_sel=0,1,2,3;fbk_sel_real=1,2,4,4
		params->dsi.fbk_div =26;//38;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	26=>351MHz Frame check=57
#else
		params->dsi.PLL_CLOCK=205;
#endif
		/* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response. 
		params->dsi.lcm_int_te_monitor = FALSE; 
		params->dsi.lcm_int_te_period = 1; // Unit : frames 
 
		// Need longer FP for more opportunity to do int. TE monitor applicably. 
		if(params->dsi.lcm_int_te_monitor) 
			params->dsi.vertical_frontporch *= 2; 
 
		// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.) 
		//params->dsi.lcm_ext_te_monitor = FALSE; 
		// Non-continuous clock 
	//	params->dsi.noncont_clock = TRUE; 
		//params->dsi.noncont_clock_period = 2; // Unit : frames
}


//add by hyde for debug
static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(10); 
    SET_RESET_PIN(0);
    MDELAY(30); 
 
    SET_RESET_PIN(1);
    MDELAY(120); 

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}



static void lcm_suspend(void)
{

    push_table(lcm_sleep_mode_in_setting, sizeof(lcm_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(20);


}


static void lcm_resume(void)
{
	lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}
         
#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1;

    unsigned char x0_MSB = ((x0>>8)&0xFF);
    unsigned char x0_LSB = (x0&0xFF);
    unsigned char x1_MSB = ((x1>>8)&0xFF);
    unsigned char x1_LSB = (x1&0xFF);
    unsigned char y0_MSB = ((y0>>8)&0xFF);
    unsigned char y0_LSB = (y0&0xFF);
    unsigned char y1_MSB = ((y1>>8)&0xFF);
    unsigned char y1_LSB = (y1&0xFF);

    unsigned int data_array[16];

    data_array[0]= 0x00053902;
    data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
    data_array[2]= (x1_LSB);
    dsi_set_cmdq(data_array, 3, 1);
    
    data_array[0]= 0x00053902;
    data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
    data_array[2]= (y1_LSB);
    dsi_set_cmdq(data_array, 3, 1);

    data_array[0]= 0x00290508; //HW bug, so need send one HS packet
    dsi_set_cmdq(data_array, 1, 1);
    
    data_array[0]= 0x002c3909;
    dsi_set_cmdq(data_array, 1, 0);

}
#endif


static unsigned int lcm_compare_id(void)
{
	unsigned int id1 = 0, id2 = 0, id3 = 0, id4 = 0,id;
	unsigned char buffer[4];
	unsigned int data_array[16];
	 
	SET_RESET_PIN(1);	//NOTE:should reset LCM firstly
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(30);
	SET_RESET_PIN(1);
	MDELAY(100);


	data_array[0] = 0x00043902;
	data_array[1] = 0xF26191BF;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(10);


	data_array[0] = 0x00023700;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(10);

	read_reg_v2(0x04, buffer, 2);
	id1= buffer[0]; //should be 
	id2= buffer[1]; //should be 0x98
	
	id=(id1 << 8) | id2;

#if defined(BUILD_LK)
	printf("JD9161 id=%x, %x %x %x %x\n", id, id1, id2, id3, id4);
#else
	printk("JD9161 id=%x, %x %x %x %x\n", id, id1, id2, id3, id4);
#endif
//	return 1;
	return (LCM_ID == id) ? 1 : 0;
}
/*
static unsigned int lcm_esd_check(void)
{
  #ifndef BUILD_LK
    char  buffer[3];
    int   array[4];

    if(lcm_esd_test)
    {
        lcm_esd_test = FALSE;
        return TRUE;
    }

    array[0] = 0x00013700;
    dsi_set_cmdq(array, 1, 1);

    read_reg_v2(0x36, buffer, 1);
    if(buffer[0]==0x90)
    {
        return FALSE;
    }
    else
    {            
        return TRUE;
    }
 #endif

}
*/
static unsigned int lcm_esd_recover(void)
{
    lcm_init();
    lcm_resume();

    return TRUE;
}


LCM_DRIVER jd9161_fwvga_dsi_vdo_auo = 
{
.name = "jd9161_fwvga_dsi_vdo_auo",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
    //.esd_check = lcm_esd_check,
    //.esd_recover = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
    };
