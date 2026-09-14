/* ===================== ADC1 (base 0x40012000) ===================== */
#define ADC_SR_EOC        (1 << 1)    /* End Of Conversion */
#define ADC_CR1_RES_Pos   24           /* độ phân giải, 00=12bit(mặc định) */
#define ADC_CR2_ADON      (1 << 0)    /* bật ADC */
#define ADC_CR2_SWSTART   (1 << 30)   /* kích hoạt 1 lần chuyển đổi bằng phần mềm */

#define ADC_SQR3_SQ1_Pos  0            /* kênh đầu tiên trong chuỗi chuyển đổi, bits [4:0] */
#define ADC_SMPR2_SMP1_Pos   3         /* thời gian sample cho kênh 1, bits [5:3] */

typedef struct 
{
    volatile uint32_t SMPR2;
    volatile uint32_t SQR3;
    volatile uint32_t SQR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
}ADC1_TypeDef;
uint16_t ADC1_ReadBatteryVoltage_mV(void);
