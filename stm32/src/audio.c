#if(0)

#include "stm32l476xx.h"
#include <stdint.h>



	/* Set up SAI for 16-bit stereo i2s */
void SAI_Init(void) {

	/* Enable the clock for SAI1 */
	RCC->APB2ENR |= RCC_APB2ENR_SAI1EN;

	/* Enable i2s mode */

	/* GCR register */
		/* ASYNCHRONOUS */
		/* SYNCEXT_DISABLE */

	/* CR1 register */

		/* Disable SAI while configuring */
		SAI1_Block_A->CR1 &= ~SAI_xCR1_SAIEN;

		/* Set "Free" Protocol */
		SAI1_Block_A->CR1&=~SAI_xCR1_PRTCFG;

		/* Set Master Transmitter */
		SAI1_Block_A->CR1&=~SAI_xCR1_MODE;

		/* Set first bit MSB */
		SAI1_Block_A->CR1&=~SAI_xCR1_LSBFIRST;

		/* Transmit, so set clock strobing to falling edge */
		SAI1_Block_A->CR1|= SAI_xCR1_CKSTR;

		/* Set datasize to 32 */
		SAI1_Block_A->CR1|=SAI_xCR1_DS;

		/* Set MCKDIV to 44.1 kHz */
		SAI1_Block_A->CR1&=~(SAI_xCR1_MCKDIV);

	/* CR2 register */

		/* Disable compounding */
		/* Disable tristate */

		/* Set FIFO Threshold to 1/4 full */
		SAI1_Block_A->CR2&=~SAI_xCR2_FTH;
		SAI1_Block_A->CR2|=1;


	/* FRCR register */

		/* Frame Length = 32, manual says subtract 1 */
		SAI1_Block_A->FRCR|= (32-1);  /* two 16-bit values */

		/* Frame offset before first bit */
		SAI1_Block_A->FRCR|= SAI_xFRCR_FSOFF;

		/* SAI_FS_CHANNEL_IDENTIFICATION */
		SAI1_Block_A->FRCR|= SAI_xFRCR_FSDEF;

		/* Frame Polarity active low*/
		SAI1_Block_A->FRCR&=~SAI_xFRCR_FSPOL;

		/* ActiveFrameLength = 16, manual says subtract 1*/
		SAI1_Block_A->FRCR|=(16-1)<<8;

	/* SLOTR register */

		/* Slot first bit offset = 0 */
		SAI1_Block_A->SLOTR&=~SAI_xSLOTR_FBOFF;

		/* Slot size = 16bits */
		SAI1_Block_A->SLOTR&=~SAI_xSLOTR_SLOTSZ;
		SAI1_Block_A->SLOTR|= 1<<6;

		/* Slot active = 0x3 */
		/* we had? SAI_SLOTACTIVE_ALL */
		//SAI1_Block_A->SLOTR|= (0xffff<<16);
		SAI1_Block_A->SLOTR|= (0x3<<16);


		/* Slot number = 2 (stereo) */
		/* ??? manual says +1 ? */
		SAI1_Block_A->SLOTR&=~SAI_xSLOTR_NBSLOT;
		SAI1_Block_A->SLOTR|= (2-1)<<8;


	/* Enable */
	SAI1_Block_A->CR1 |= SAI_xCR1_SAIEN;

}




#define NULL 0

/* Blocking transmit */
int i2s_transmit(uint8_t *datal, uint8_t *datar, uint16_t size) {

	uint32_t count=size;
	uint32_t temp;
	uint8_t *left_data=datal;
	uint8_t *right_data=datar;
	int left=1;

	if ((datar==NULL) || (datal==NULL) || (size==0)) return -1;

	while (count > 0U) {
		/* Write data if the FIFO is not full */
		if ((SAI1_Block_A->SR & SAI_xSR_FLVL) != SAI_xSR_FLVL) {
			if (left) {
				temp = (uint32_t)(*left_data);
				left_data++;
				temp |= ((uint32_t)(*left_data) << 8);
				left_data++;
				left=0;
			}
			else {
				temp = (uint32_t)(*right_data);
				right_data++;
				temp |= ((uint32_t)(*right_data) << 8);
				right_data++;
				left=1;
			}
			SAI1_Block_A->DR = temp;
			count--;
			GPIOE->ODR &= ~(1<<8);
		}
		else {
			GPIOE->ODR |= (1<<8);
		}
	}

	/* Check for the Timeout */

	return 0;
}





int main(void) {


	volatile int d;

	/* Set up i2c */

	uint8_t data_receive[6];
	uint8_t data_send[6];
	int slave_addr;

	data_receive[0]=0;

	i2c_init(I2C1);

	SAI_Init();

	/* Set up cs43l22 */
	cs43l22_init();

	slave_addr=0x94;
	data_send[0]=CS43L22_REG_ID;
	i2c_send_data(I2C1,slave_addr,data_send,1);
	i2c_receive_data(I2C1,slave_addr,data_receive,1);





  // Init cs43l22 over i2c
	cs43l22_play();




	while(1) {
		i2s_transmit(audio_buf,audio_buf,AUDIO_BUFSIZ);
	}


}

#endif
