
//------------------------------------------------------
//
// FILENAME : DNM_NIST.H
//
// 
// AUTHOR   : C Tang
// 
//------------------------------------------------------







//MAC ID definition
#define  DNM_DEVICE_MACID_0			0
#define  DNM_DEVICE_MACID_1			1
#define  DNM_DEVICE_MACID_2			2
#define  DNM_DEVICE_MACID_3			3
#define  DNM_DEVICE_MACID_4			4
#define  DNM_DEVICE_MACID_5			5
#define  DNM_DEVICE_MACID_6			6
#define  DNM_DEVICE_MACID_7			7
#define  DNM_DEVICE_MACID_8			8
#define  DNM_DEVICE_MACID_9			9

#define  DNM_DEVICE_MACID_10		10
#define  DNM_DEVICE_MACID_11		11
#define  DNM_DEVICE_MACID_12		12
#define  DNM_DEVICE_MACID_13		13
#define  DNM_DEVICE_MACID_14		14
#define  DNM_DEVICE_MACID_15		15
#define  DNM_DEVICE_MACID_16		16
#define  DNM_DEVICE_MACID_17		17
#define  DNM_DEVICE_MACID_18		18
#define  DNM_DEVICE_MACID_19		19

#define  DNM_DEVICE_MACID_20		20
#define  DNM_DEVICE_MACID_21		21
#define  DNM_DEVICE_MACID_22		22
#define  DNM_DEVICE_MACID_23		23
#define  DNM_DEVICE_MACID_24		24
#define  DNM_DEVICE_MACID_25		25
#define  DNM_DEVICE_MACID_26		26
#define  DNM_DEVICE_MACID_27		27
#define  DNM_DEVICE_MACID_28		28
#define  DNM_DEVICE_MACID_29		29

#define  DNM_DEVICE_MACID_30		30
#define  DNM_DEVICE_MACID_31		31
#define  DNM_DEVICE_MACID_32		32
#define  DNM_DEVICE_MACID_33		33
#define  DNM_DEVICE_MACID_34		34
#define  DNM_DEVICE_MACID_35		35
#define  DNM_DEVICE_MACID_36		36
#define  DNM_DEVICE_MACID_37		37
#define  DNM_DEVICE_MACID_38		38
#define  DNM_DEVICE_MACID_39		39

#define  DNM_DEVICE_MACID_40		40
#define  DNM_DEVICE_MACID_41		41
#define  DNM_DEVICE_MACID_42		42
#define  DNM_DEVICE_MACID_43		43
#define  DNM_DEVICE_MACID_44		44
#define  DNM_DEVICE_MACID_45		45
#define  DNM_DEVICE_MACID_46		46
#define  DNM_DEVICE_MACID_47		47
#define  DNM_DEVICE_MACID_48		48
#define  DNM_DEVICE_MACID_49		49

#define  DNM_DEVICE_MACID_50		50
#define  DNM_DEVICE_MACID_51		51
#define  DNM_DEVICE_MACID_52		52
#define  DNM_DEVICE_MACID_53		53
#define  DNM_DEVICE_MACID_54		54
#define  DNM_DEVICE_MACID_55		55
#define  DNM_DEVICE_MACID_56		56
#define  DNM_DEVICE_MACID_57		57
#define  DNM_DEVICE_MACID_58		58
#define  DNM_DEVICE_MACID_59		59

#define  DNM_DEVICE_MACID_60		60
#define  DNM_DEVICE_MACID_61		61
#define  DNM_DEVICE_MACID_62		62
#define  DNM_DEVICE_MACID_63		63


//TE Sim specific defines
#define	TE_SIM_NUM_XMEAS				41
#define	TE_SIM_NUM_XMV					12
#define TE_SIM_XMEAS_SHMEM_NAME			("sim_shmem")
#define TE_SIM_XMV_SHMEM_NAME			("xmv_shmem")

//Function Prototype
void create_shm_tesim_xmeas(void);
void map_shm_tesim_xmeas(void);
void map_shm_tesim_xmv(void);