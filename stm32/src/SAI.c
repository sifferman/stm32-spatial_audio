
#include "SAI.h"
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

		/* Set datasize to 16 (bit depth) */
		SAI1_Block_A->CR1&=~SAI_xCR1_DS;
    SAI1_Block_A->CR1|= 4<<5;

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
/*
    SAI1_Block_A->FRCR&=~SAI_xFRCR_FSALL;
    SAI1_Block_A->FRCR|= SAI_xFRCR_FSALL_0;
*/
		/* SAI_FS_CHANNEL_IDENTIFICATION */
		SAI1_Block_A->FRCR|= SAI_xFRCR_FSDEF;

		/* Frame Polarity active low*/
		SAI1_Block_A->FRCR&=~SAI_xFRCR_FSPOL;

		/* ActiveFrameLength = 16, manual says subtract 1*/
		SAI1_Block_A->FRCR|=(16-1)<<8;

	/* SLOTR register */

		/* Slot first bit offset = 0 */
		SAI1_Block_A->SLOTR&=~SAI_xSLOTR_FBOFF;

		/* Slot size = data size */
		SAI1_Block_A->SLOTR&=~SAI_xSLOTR_SLOTSZ;

		/* Slot active = 0x3 */
		/* we had? SAI_SLOTACTIVE_ALL */
		SAI1_Block_A->SLOTR|=SAI_xSLOTR_SLOTEN;
    //SAI1_Block_A->SLOTR&=~SAI_xSLOTR_SLOTEN;
    SAI1_Block_A->SLOTR|= (0x3<<16);


		/* Slot number = 2 (stereo) */
		SAI1_Block_A->SLOTR&=~SAI_xSLOTR_NBSLOT;
		//SAI1_Block_A->SLOTR|= SAI_xSLOTR_NBSLOT_1;
    SAI1_Block_A->SLOTR|= SAI_xSLOTR_NBSLOT_0;


	/* Enable */
	SAI1_Block_A->CR1 |= SAI_xCR1_SAIEN;

}
