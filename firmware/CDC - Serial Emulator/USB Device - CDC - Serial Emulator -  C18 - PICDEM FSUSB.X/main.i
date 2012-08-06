#line 1 "../main.c"
#line 1 "../main.c"

#line 42 "../main.c"
 

 
#line 1 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

#line 45 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
 


#line 49 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

 
#line 52 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 54 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 55 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 56 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

#line 58 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 59 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 60 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

 
#line 1 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"
 

#line 4 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"

typedef unsigned char wchar_t;


#line 10 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"
 
typedef signed short int ptrdiff_t;
typedef signed short int ptrdiffram_t;
typedef signed short long int ptrdiffrom_t;


#line 20 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"
 
typedef unsigned short int size_t;
typedef unsigned short int sizeram_t;
typedef unsigned short long int sizerom_t;


#line 34 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"
 
#line 36 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"


#line 41 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"
 
#line 43 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"

#line 45 "/opt/microchip/mplabc18/v3.40/bin/../h/stddef.h"
#line 62 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
 

typedef enum _BOOL { FALSE = 0, TRUE } BOOL;     
typedef enum _BIT { CLEAR = 0, SET } BIT;

#line 68 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 69 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 70 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

 
typedef signed int          INT;
typedef signed char         INT8;
typedef signed short int    INT16;
typedef signed long int     INT32;

 
#line 79 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 81 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

 
typedef unsigned int        UINT;
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
 
#line 88 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
typedef unsigned short long UINT24;
#line 90 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
typedef unsigned long int   UINT32;      
 
#line 93 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 95 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

typedef union
{
    UINT8 Val;
    struct
    {
         UINT8 b0:1;
         UINT8 b1:1;
         UINT8 b2:1;
         UINT8 b3:1;
         UINT8 b4:1;
         UINT8 b5:1;
         UINT8 b6:1;
         UINT8 b7:1;
    } bits;
} UINT8_VAL, UINT8_BITS;

typedef union 
{
    UINT16 Val;
    UINT8 v[2] ;
    struct 
    {
        UINT8 LB;
        UINT8 HB;
    } byte;
    struct 
    {
         UINT8 b0:1;
         UINT8 b1:1;
         UINT8 b2:1;
         UINT8 b3:1;
         UINT8 b4:1;
         UINT8 b5:1;
         UINT8 b6:1;
         UINT8 b7:1;
         UINT8 b8:1;
         UINT8 b9:1;
         UINT8 b10:1;
         UINT8 b11:1;
         UINT8 b12:1;
         UINT8 b13:1;
         UINT8 b14:1;
         UINT8 b15:1;
    } bits;
} UINT16_VAL, UINT16_BITS;

 
#line 144 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
typedef union
{
    UINT24 Val;
    UINT8 v[3] ;
    struct 
    {
        UINT8 LB;
        UINT8 HB;
        UINT8 UB;
    } byte;
    struct 
    {
         UINT8 b0:1;
         UINT8 b1:1;
         UINT8 b2:1;
         UINT8 b3:1;
         UINT8 b4:1;
         UINT8 b5:1;
         UINT8 b6:1;
         UINT8 b7:1;
         UINT8 b8:1;
         UINT8 b9:1;
         UINT8 b10:1;
         UINT8 b11:1;
         UINT8 b12:1;
         UINT8 b13:1;
         UINT8 b14:1;
         UINT8 b15:1;
         UINT8 b16:1;
         UINT8 b17:1;
         UINT8 b18:1;
         UINT8 b19:1;
         UINT8 b20:1;
         UINT8 b21:1;
         UINT8 b22:1;
         UINT8 b23:1;
    } bits;
} UINT24_VAL, UINT24_BITS;
#line 183 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

typedef union
{
    UINT32 Val;
    UINT16 w[2] ;
    UINT8  v[4] ;
    struct 
    {
        UINT16 LW;
        UINT16 HW;
    } word;
    struct 
    {
        UINT8 LB;
        UINT8 HB;
        UINT8 UB;
        UINT8 MB;
    } byte;
    struct 
    {
        UINT16_VAL low;
        UINT16_VAL high;
    }wordUnion;
    struct 
    {
         UINT8 b0:1;
         UINT8 b1:1;
         UINT8 b2:1;
         UINT8 b3:1;
         UINT8 b4:1;
         UINT8 b5:1;
         UINT8 b6:1;
         UINT8 b7:1;
         UINT8 b8:1;
         UINT8 b9:1;
         UINT8 b10:1;
         UINT8 b11:1;
         UINT8 b12:1;
         UINT8 b13:1;
         UINT8 b14:1;
         UINT8 b15:1;
         UINT8 b16:1;
         UINT8 b17:1;
         UINT8 b18:1;
         UINT8 b19:1;
         UINT8 b20:1;
         UINT8 b21:1;
         UINT8 b22:1;
         UINT8 b23:1;
         UINT8 b24:1;
         UINT8 b25:1;
         UINT8 b26:1;
         UINT8 b27:1;
         UINT8 b28:1;
         UINT8 b29:1;
         UINT8 b30:1;
         UINT8 b31:1;
    } bits;
} UINT32_VAL;

 
#line 245 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 332 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

 

 
typedef void                    VOID;

typedef char                    CHAR8;
typedef unsigned char           UCHAR8;

typedef unsigned char           BYTE;                            
typedef unsigned short int      WORD;                            
typedef unsigned long           DWORD;                           
 

typedef unsigned long long      QWORD;                           
typedef signed char             CHAR;                            
typedef signed short int        SHORT;                           
typedef signed long             LONG;                            
 

typedef signed long long        LONGLONG;                        
typedef union
{
    BYTE Val;
    struct 
    {
         BYTE b0:1;
         BYTE b1:1;
         BYTE b2:1;
         BYTE b3:1;
         BYTE b4:1;
         BYTE b5:1;
         BYTE b6:1;
         BYTE b7:1;
    } bits;
} BYTE_VAL, BYTE_BITS;

typedef union
{
    WORD Val;
    BYTE v[2] ;
    struct 
    {
        BYTE LB;
        BYTE HB;
    } byte;
    struct 
    {
         BYTE b0:1;
         BYTE b1:1;
         BYTE b2:1;
         BYTE b3:1;
         BYTE b4:1;
         BYTE b5:1;
         BYTE b6:1;
         BYTE b7:1;
         BYTE b8:1;
         BYTE b9:1;
         BYTE b10:1;
         BYTE b11:1;
         BYTE b12:1;
         BYTE b13:1;
         BYTE b14:1;
         BYTE b15:1;
    } bits;
} WORD_VAL, WORD_BITS;

typedef union
{
    DWORD Val;
    WORD w[2] ;
    BYTE v[4] ;
    struct 
    {
        WORD LW;
        WORD HW;
    } word;
    struct 
    {
        BYTE LB;
        BYTE HB;
        BYTE UB;
        BYTE MB;
    } byte;
    struct 
    {
        WORD_VAL low;
        WORD_VAL high;
    }wordUnion;
    struct 
    {
         BYTE b0:1;
         BYTE b1:1;
         BYTE b2:1;
         BYTE b3:1;
         BYTE b4:1;
         BYTE b5:1;
         BYTE b6:1;
         BYTE b7:1;
         BYTE b8:1;
         BYTE b9:1;
         BYTE b10:1;
         BYTE b11:1;
         BYTE b12:1;
         BYTE b13:1;
         BYTE b14:1;
         BYTE b15:1;
         BYTE b16:1;
         BYTE b17:1;
         BYTE b18:1;
         BYTE b19:1;
         BYTE b20:1;
         BYTE b21:1;
         BYTE b22:1;
         BYTE b23:1;
         BYTE b24:1;
         BYTE b25:1;
         BYTE b26:1;
         BYTE b27:1;
         BYTE b28:1;
         BYTE b29:1;
         BYTE b30:1;
         BYTE b31:1;
    } bits;
} DWORD_VAL;

 
typedef union
{
    QWORD Val;
    DWORD d[2] ;
    WORD w[4] ;
    BYTE v[8] ;
    struct 
    {
        DWORD LD;
        DWORD HD;
    } dword;
    struct 
    {
        WORD LW;
        WORD HW;
        WORD UW;
        WORD MW;
    } word;
    struct 
    {
         BYTE b0:1;
         BYTE b1:1;
         BYTE b2:1;
         BYTE b3:1;
         BYTE b4:1;
         BYTE b5:1;
         BYTE b6:1;
         BYTE b7:1;
         BYTE b8:1;
         BYTE b9:1;
         BYTE b10:1;
         BYTE b11:1;
         BYTE b12:1;
         BYTE b13:1;
         BYTE b14:1;
         BYTE b15:1;
         BYTE b16:1;
         BYTE b17:1;
         BYTE b18:1;
         BYTE b19:1;
         BYTE b20:1;
         BYTE b21:1;
         BYTE b22:1;
         BYTE b23:1;
         BYTE b24:1;
         BYTE b25:1;
         BYTE b26:1;
         BYTE b27:1;
         BYTE b28:1;
         BYTE b29:1;
         BYTE b30:1;
         BYTE b31:1;
         BYTE b32:1;
         BYTE b33:1;
         BYTE b34:1;
         BYTE b35:1;
         BYTE b36:1;
         BYTE b37:1;
         BYTE b38:1;
         BYTE b39:1;
         BYTE b40:1;
         BYTE b41:1;
         BYTE b42:1;
         BYTE b43:1;
         BYTE b44:1;
         BYTE b45:1;
         BYTE b46:1;
         BYTE b47:1;
         BYTE b48:1;
         BYTE b49:1;
         BYTE b50:1;
         BYTE b51:1;
         BYTE b52:1;
         BYTE b53:1;
         BYTE b54:1;
         BYTE b55:1;
         BYTE b56:1;
         BYTE b57:1;
         BYTE b58:1;
         BYTE b59:1;
         BYTE b60:1;
         BYTE b61:1;
         BYTE b62:1;
         BYTE b63:1;
    } bits;
} QWORD_VAL;

#line 547 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

#line 549 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 45 "../main.c"


#line 1 "../usb_config.h"

#line 43 "../usb_config.h"
 


#line 47 "../usb_config.h"
 


#line 51 "../usb_config.h"

 
#line 54 "../usb_config.h"
								
								
								
								
								
									
#line 61 "../usb_config.h"




#line 66 "../usb_config.h"
#line 67 "../usb_config.h"



#line 71 "../usb_config.h"
#line 72 "../usb_config.h"



#line 76 "../usb_config.h"





#line 82 "../usb_config.h"

 
#line 85 "../usb_config.h"


#line 88 "../usb_config.h"


#line 91 "../usb_config.h"



 
#line 96 "../usb_config.h"
#line 97 "../usb_config.h"

 
#line 100 "../usb_config.h"

 
#line 103 "../usb_config.h"
#line 104 "../usb_config.h"
#line 105 "../usb_config.h"

#line 107 "../usb_config.h"
#line 108 "../usb_config.h"
#line 109 "../usb_config.h"
#line 110 "../usb_config.h"

#line 112 "../usb_config.h"



#line 116 "../usb_config.h"
 

#line 119 "../usb_config.h"
#line 47 "../main.c"





#line 1 "../HardwareProfile.h"

#line 43 "../HardwareProfile.h"
 


#line 47 "../HardwareProfile.h"



#line 51 "../HardwareProfile.h"
#line 52 "../HardwareProfile.h"
#line 53 "../HardwareProfile.h"
#line 61 "../HardwareProfile.h"
#line 62 "../HardwareProfile.h"

#line 64 "../HardwareProfile.h"
#line 65 "../HardwareProfile.h"
#line 66 "../HardwareProfile.h"
#line 67 "../HardwareProfile.h"
#line 68 "../HardwareProfile.h"
#line 69 "../HardwareProfile.h"
#line 73 "../HardwareProfile.h"
#line 77 "../HardwareProfile.h"
#line 79 "../HardwareProfile.h"
#line 80 "../HardwareProfile.h"
#line 81 "../HardwareProfile.h"
#line 82 "../HardwareProfile.h"
#line 83 "../HardwareProfile.h"
#line 84 "../HardwareProfile.h"

#line 86 "../HardwareProfile.h"
#line 88 "../HardwareProfile.h"



 
#line 93 "../HardwareProfile.h"
#line 94 "../HardwareProfile.h"

 
#line 97 "../HardwareProfile.h"
	
	
	
	
	

	
#line 105 "../HardwareProfile.h"

#line 107 "../HardwareProfile.h"
    
#line 109 "../HardwareProfile.h"
#line 111 "../HardwareProfile.h"
#line 112 "../HardwareProfile.h"
#line 113 "../HardwareProfile.h"
    
#line 115 "../HardwareProfile.h"
    
#line 117 "../HardwareProfile.h"
#line 119 "../HardwareProfile.h"
#line 120 "../HardwareProfile.h"
#line 121 "../HardwareProfile.h"
    
    
#line 124 "../HardwareProfile.h"
#line 125 "../HardwareProfile.h"
#line 126 "../HardwareProfile.h"
#line 127 "../HardwareProfile.h"
#line 128 "../HardwareProfile.h"
#line 129 "../HardwareProfile.h"
    
#line 131 "../HardwareProfile.h"
    
     
#line 134 "../HardwareProfile.h"
    
#line 136 "../HardwareProfile.h"
#line 137 "../HardwareProfile.h"
#line 138 "../HardwareProfile.h"
#line 139 "../HardwareProfile.h"
    
#line 141 "../HardwareProfile.h"
#line 142 "../HardwareProfile.h"
#line 143 "../HardwareProfile.h"
#line 144 "../HardwareProfile.h"
    
#line 146 "../HardwareProfile.h"
#line 147 "../HardwareProfile.h"
#line 148 "../HardwareProfile.h"
#line 149 "../HardwareProfile.h"
    
#line 151 "../HardwareProfile.h"
#line 152 "../HardwareProfile.h"
#line 153 "../HardwareProfile.h"
#line 154 "../HardwareProfile.h"
    
     
#line 157 "../HardwareProfile.h"
#line 158 "../HardwareProfile.h"
#line 159 "../HardwareProfile.h"
#line 160 "../HardwareProfile.h"
#line 161 "../HardwareProfile.h"
    
     
#line 164 "../HardwareProfile.h"
    
     
#line 167 "../HardwareProfile.h"
#line 168 "../HardwareProfile.h"
    
#line 170 "../HardwareProfile.h"
#line 171 "../HardwareProfile.h"
    
     
#line 174 "../HardwareProfile.h"
#line 175 "../HardwareProfile.h"
    
#line 177 "../HardwareProfile.h"
#line 178 "../HardwareProfile.h"

	 
#line 181 "../HardwareProfile.h"
#line 182 "../HardwareProfile.h"
#line 183 "../HardwareProfile.h"
#line 184 "../HardwareProfile.h"
#line 185 "../HardwareProfile.h"
#line 186 "../HardwareProfile.h"
#line 187 "../HardwareProfile.h"
#line 188 "../HardwareProfile.h"
#line 189 "../HardwareProfile.h"
#line 190 "../HardwareProfile.h"

#line 192 "../HardwareProfile.h"
#line 205 "../HardwareProfile.h"
#line 207 "../HardwareProfile.h"
#line 209 "../HardwareProfile.h"
#line 213 "../HardwareProfile.h"
#line 215 "../HardwareProfile.h"
#line 217 "../HardwareProfile.h"

#line 264 "../HardwareProfile.h"
#line 298 "../HardwareProfile.h"

#line 300 "../HardwareProfile.h"
#line 311 "../HardwareProfile.h"
#line 313 "../HardwareProfile.h"
#line 315 "../HardwareProfile.h"
#line 319 "../HardwareProfile.h"
#line 321 "../HardwareProfile.h"
#line 323 "../HardwareProfile.h"
#line 393 "../HardwareProfile.h"

#line 395 "../HardwareProfile.h"
#line 459 "../HardwareProfile.h"

#line 461 "../HardwareProfile.h"
#line 52 "../main.c"


#line 55 "../main.c"
#line 57 "../main.c"

 
#line 60 "../main.c"
        #pragma config PLLDIV   = 5         
        #pragma config CPUDIV   = OSC1_PLL2   
        #pragma config USBDIV   = 2         
        #pragma config FOSC     = HSPLL_HS
        #pragma config FCMEN    = OFF
        #pragma config IESO     = OFF
        #pragma config PWRT     = OFF
        #pragma config BOR      = ON
        #pragma config BORV     = 3
        #pragma config VREGEN   = ON      
        #pragma config WDT      = OFF
        #pragma config WDTPS    = 32768
        #pragma config MCLRE    = ON
        #pragma config LPT1OSC  = OFF
        #pragma config PBADEN   = OFF

        #pragma config STVREN   = ON
        #pragma config LVP      = OFF

        #pragma config XINST    = OFF       
        #pragma config CP0      = OFF
        #pragma config CP1      = OFF


        #pragma config CPB      = OFF

        #pragma config WRT0     = OFF
        #pragma config WRT1     = OFF


        #pragma config WRTB     = OFF       
        #pragma config WRTC     = OFF

        #pragma config EBTR0    = OFF
        #pragma config EBTR1    = OFF


        #pragma config EBTRB    = OFF


#line 101 "../main.c"
#line 121 "../main.c"
#line 141 "../main.c"
#line 144 "../main.c"
#line 146 "../main.c"
#line 150 "../main.c"
#line 152 "../main.c"
#line 153 "../main.c"
#line 155 "../main.c"

 

#line 1 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"

#line 45 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
 

#line 52 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 54 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 56 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 58 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 60 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 79 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 81 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 88 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 90 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 93 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 95 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 144 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 183 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 245 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 332 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 549 "/opt/microchip/mplabc18/v3.40/bin/../h/GenericTypeDefs.h"
#line 158 "../main.c"


#line 1 "../usb_config.h"

#line 43 "../usb_config.h"
 


#line 47 "../usb_config.h"
 

#line 119 "../usb_config.h"
#line 160 "../main.c"




#line 1 "../HardwareProfile.h"

#line 43 "../HardwareProfile.h"
 

#line 51 "../HardwareProfile.h"
#line 52 "../HardwareProfile.h"
#line 53 "../HardwareProfile.h"
#line 61 "../HardwareProfile.h"
#line 62 "../HardwareProfile.h"
#line 64 "../HardwareProfile.h"
#line 65 "../HardwareProfile.h"
#line 69 "../HardwareProfile.h"
#line 73 "../HardwareProfile.h"
#line 77 "../HardwareProfile.h"
#line 79 "../HardwareProfile.h"
#line 80 "../HardwareProfile.h"
#line 83 "../HardwareProfile.h"
#line 84 "../HardwareProfile.h"
#line 86 "../HardwareProfile.h"
#line 88 "../HardwareProfile.h"
#line 97 "../HardwareProfile.h"
#line 109 "../HardwareProfile.h"
#line 111 "../HardwareProfile.h"
#line 113 "../HardwareProfile.h"
#line 117 "../HardwareProfile.h"
#line 119 "../HardwareProfile.h"
#line 121 "../HardwareProfile.h"
#line 190 "../HardwareProfile.h"
#line 192 "../HardwareProfile.h"
#line 205 "../HardwareProfile.h"
#line 207 "../HardwareProfile.h"
#line 209 "../HardwareProfile.h"
#line 213 "../HardwareProfile.h"
#line 215 "../HardwareProfile.h"
#line 217 "../HardwareProfile.h"

#line 264 "../HardwareProfile.h"
#line 298 "../HardwareProfile.h"
#line 300 "../HardwareProfile.h"
#line 311 "../HardwareProfile.h"
#line 313 "../HardwareProfile.h"
#line 315 "../HardwareProfile.h"
#line 319 "../HardwareProfile.h"
#line 321 "../HardwareProfile.h"
#line 323 "../HardwareProfile.h"
#line 393 "../HardwareProfile.h"
#line 395 "../HardwareProfile.h"
#line 459 "../HardwareProfile.h"
#line 461 "../HardwareProfile.h"
#line 164 "../main.c"


 
#pragma udata
char USB_Out_Buffer[64 ];
char RS232_Out_Data[64 ];

unsigned char  NextUSBOut;
unsigned char    NextUSBOut;

unsigned char    LastRS232Out;  
unsigned char    RS232cp;       
unsigned char RS232_Out_Data_Rdy = 0;
USB_HANDLE  lastTransmission;




 
static void InitializeSystem(void);
void ProcessIO(void);
void USBDeviceTasks(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void BlinkUSBStatus(void);
void UserInit(void);
void InitializeUSART(void);
void putcUSART(char c);
unsigned char getcUSART ();

 
#line 196 "../main.c"
	
	
	
	
	
	
	
	
	
	
	
	
	
#line 210 "../main.c"
#line 211 "../main.c"
#line 212 "../main.c"
#line 213 "../main.c"
#line 214 "../main.c"
#line 218 "../main.c"
#line 222 "../main.c"
	
#line 224 "../main.c"
	extern void _startup (void);        
	#pragma code REMAPPED_RESET_VECTOR = 0x1000 
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}
#line 231 "../main.c"
	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = 0x1008 
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = 0x1018 
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	
#line 243 "../main.c"
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	#pragma code HIGH_INTERRUPT_VECTOR = 0x08
	void High_ISR (void)
	{
	     _asm goto 0x1008  _endasm
	}
	#pragma code LOW_INTERRUPT_VECTOR = 0x18
	void Low_ISR (void)
	{
	     _asm goto 0x1018  _endasm
	}
#line 271 "../main.c"

	#pragma code
	
	
	
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		
		
		
		
	
	}	
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		
		
		
		
	
	}	

#line 296 "../main.c"




 
#pragma code


#line 317 "../main.c"
 
#line 319 "../main.c"
void main(void)
#line 321 "../main.c"
#line 323 "../main.c"
{   
    InitializeSystem();

    while(1)
    {
		
        USBDeviceTasks(); 
        				  
        				  
        				  
        				  
        				  
        				  
        				  
        				  
        				  
        				  
        				  
    				  

		
		
        ProcessIO();        
    }
}



#line 369 "../main.c"
 
void InitializeSystem(void)
{
#line 373 "../main.c"
        ADCON1 |= 0x0F;                 
#line 375 "../main.c"
#line 377 "../main.c"

#line 379 "../main.c"
#line 400 "../main.c"
    

















#line 419 "../main.c"
#line 421 "../main.c"
    












#line 435 "../main.c"
#line 437 "../main.c"
    
    USBDeviceInit();	
    					
    UserInit();

}




#line 462 "../main.c"
 
void UserInit(void)
{
	unsigned char i;
    InitializeUSART();


	for (i=0; i<sizeof(USB_Out_Buffer); i++)
    {
		USB_Out_Buffer[i] = 0;
    }

	NextUSBOut = 0;
	LastRS232Out = 0;
	lastTransmission = 0;

}


#line 495 "../main.c"
 
void InitializeUSART(void)
{
#line 499 "../main.c"
	    unsigned char c;
#line 501 "../main.c"
#line 503 "../main.c"
        TRISBbits.TRISB5 =1;				
        TRISBbits.TRISB7 =0;				
        TXSTA = 0x24;       	
        RCSTA = 0x90;       	
        SPBRG = 0x70;
        SPBRGH = 0x02;      	
        BAUDCON = 0x08;     	
        c = RCREG;				
#line 512 "../main.c"

#line 514 "../main.c"
#line 515 "../main.c"
#line 521 "../main.c"
#line 523 "../main.c"
#line 526 "../main.c"

}

#line 530 "../main.c"
#line 531 "../main.c"
#line 532 "../main.c"
#line 533 "../main.c"
#line 536 "../main.c"


#line 552 "../main.c"
 
void putcUSART(char c)  
{
#line 556 "../main.c"
	    TXREG = c;
#line 558 "../main.c"
#line 560 "../main.c"
}



#line 581 "../main.c"
 
#line 583 "../main.c"
void mySetLineCodingHandler(void)
{
    
    if(cdc_notice.GetLineCoding.dwDTERate.Val > 115200)
    {
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
    }
    else
    {
        DWORD_VAL dwBaud;

        
        CDCSetBaudRate(cdc_notice.GetLineCoding.dwDTERate.Val);
        
        
#line 613 "../main.c"
        dwBaud.Val = (48000000 /4)/line_coding.dwDTERate.Val-1;
        SPBRG = dwBaud.v[0];
        SPBRGH = dwBaud.v[1];
#line 617 "../main.c"
#line 620 "../main.c"
    }
}
#line 623 "../main.c"


#line 639 "../main.c"
 
unsigned char getcUSART ()
{
	char  c;

#line 645 "../main.c"

	if (RCSTAbits.OERR)  
	{                    
		RCSTAbits.CREN = 0;  
		c = RCREG;
		RCSTAbits.CREN = 1;  
	}
	else
		c = RCREG;



#line 658 "../main.c"

#line 660 "../main.c"
#line 662 "../main.c"

	return c;
}


#line 682 "../main.c"
 
void ProcessIO(void)
{   
    
    BlinkUSBStatus();
    
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	if (RS232_Out_Data_Rdy == 0)  
	{						  
		LastRS232Out = getsUSBUSART(RS232_Out_Data,64); 
		if(LastRS232Out > 0)
		{	
			RS232_Out_Data_Rdy = 1;  
			RS232cp = 0;  
		}
	}

	if(RS232_Out_Data_Rdy && TXSTAbits.TRMT )
	{
		putcUSART(RS232_Out_Data[RS232cp]);
		++RS232cp;
		if (RS232cp == LastRS232Out)
			RS232_Out_Data_Rdy = 0;
	}

	if(PIR1bits.RCIF )
	{
		USB_Out_Buffer[NextUSBOut] = getcUSART();
		++NextUSBOut;
		USB_Out_Buffer[NextUSBOut] = 0;
	}

	if((mUSBUSARTIsTxTrfReady()) && (NextUSBOut > 0))
	{
		putUSBUSART(&USB_Out_Buffer[0], NextUSBOut);
		NextUSBOut = 0;
	}

    CDCTxService();
}		


#line 741 "../main.c"
 
void BlinkUSBStatus(void)
{
    static WORD led_count=0;
    
    if(led_count == 0)led_count = 10000U;
    led_count--;

#line 750 "../main.c"
#line 751 "../main.c"
#line 752 "../main.c"
#line 753 "../main.c"

    if(USBSuspendControl == 1)
    {
        if(led_count==0)
        {
            LATDbits.LATD0  = !LATDbits.LATD0 ; ;
            LATDbits.LATD1  = LATDbits.LATD0 ;        
        }
    }
    else
    {
        if(USBDeviceState == DETACHED_STATE)
        {
            {LATDbits.LATD0  = 0; ;LATDbits.LATD1  = 0; ;} ;
        }
        else if(USBDeviceState == ATTACHED_STATE)
        {
            {LATDbits.LATD0  = 1; ;LATDbits.LATD1  = 1; ;} ;
        }
        else if(USBDeviceState == POWERED_STATE)
        {
            {LATDbits.LATD0  = 1; ;LATDbits.LATD1  = 0; ;} ;
        }
        else if(USBDeviceState == DEFAULT_STATE)
        {
            {LATDbits.LATD0  = 0; ;LATDbits.LATD1  = 1; ;} ;
        }
        else if(USBDeviceState == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                LATDbits.LATD0  = !LATDbits.LATD0 ; ;
                LATDbits.LATD1  = 0; ;
            }
        }
        else if(USBDeviceState == CONFIGURED_STATE)
        {
            if(led_count==0)
            {
                LATDbits.LATD0  = !LATDbits.LATD0 ; ;
                LATDbits.LATD1  = !LATDbits.LATD0 ;       
            }
        }
    }

}





















#line 833 "../main.c"
 
void USBCBSuspend(void)
{
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	

#line 854 "../main.c"
#line 855 "../main.c"
#line 867 "../main.c"
#line 868 "../main.c"
}



#line 887 "../main.c"
 
#line 889 "../main.c"
#line 892 "../main.c"
#line 906 "../main.c"
#line 908 "../main.c"


#line 929 "../main.c"
 
void USBCBWakeFromSuspend(void)
{
	
	
	
	
	
	
	
	
}


#line 959 "../main.c"
 
void USBCB_SOF_Handler(void)
{
    
    
}


#line 982 "../main.c"
 
void USBCBErrorHandler(void)
{
    
    

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}



#line 1033 "../main.c"
 
void USBCBCheckOtherReq(void)
{
    USBCheckCDCRequest();
}



#line 1058 "../main.c"
 
void USBCBStdSetDscHandler(void)
{
    
}



#line 1084 "../main.c"
 
void USBCBInitEP(void)
{
    CDCInitEP();
}


#line 1158 "../main.c"
 
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    USBResumeControl = 1;                
    
    delay_count = 1800U;                
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}



#line 1197 "../main.c"
 
#line 1199 "../main.c"
#line 1203 "../main.c"


 
