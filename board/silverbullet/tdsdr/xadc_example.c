/******************************************************************************
*
* (c) Copyright 2011-2013 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/****************************************************************************/
/**
*
* @file xadc_polled_printf_example.c
*
* This file contains a design example using the driver functions
* of the XADC driver. The example here shows the driver/device in polled mode
* to check the on-chip temperature and voltages.
*
* @note
*
* This examples also assumes that there is a STDIO device in the system.
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- -----  -------- -----------------------------------------------------
* 1.00a ssb    12/22/11 First release based on the XPS/AXI SysMon driver
*
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xparameters.h"
#include "xadcps.h"
#include "xstatus.h"
#include "common.h"

/************************** Constant Definitions ****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define XADC_DEVICE_ID 		XPAR_XADCPS_0_DEVICE_ID
#define XADCPS_ATR_TEST_VALUE 		0x55


/**************************** Type Definitions ******************************/


/***************** Macros (Inline Functions) Definitions ********************/


/************************** Function Prototypes *****************************/

static int XAdcPolledPrintfExample(u16 XAdcDeviceId);
static int XAdcFractionToInt(float FloatNum);

/************************** Variable Definitions ****************************/

static XAdcPs XAdcInst;      /* XADC driver instance */

/****************************************************************************/
/**
*
* Main function that invokes the polled example in this file.
*
* @param	None.
*
* @return
*		- XST_SUCCESS if the example has completed successfully.
*		- XST_FAILURE if the example has failed.
*
* @note		None.
*
*****************************************************************************/
int xadc_main(void)
{

	int Status;

	/*
	 * Run the polled example, specify the Device ID that is
	 * generated in xparameters.h.
	 */
	Status = XAdcPolledPrintfExample(XADC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}


/****************************************************************************/
/**
*
* This function runs a test on the XADC/ADC device using the
* driver APIs.
* This function does the following tasks:
*	- Initiate the XADC device driver instance
*	- Run self-test on the device
*	- Setup the sequence registers to continuously monitor on-chip
*	temperature and, VCCPINT, VCCPAUX and VCCPDRO Voltages
*	- Setup configuration registers to start the sequence
*	- Read the latest on-chip temperature and, VCCPINT, VCCPAUX and VCCPDRO
*	  Voltages
*
* @param	XAdcDeviceId is the XPAR_<SYSMON_ADC_instance>_DEVICE_ID value
*		from xparameters.h.
*
* @return
*		- XST_SUCCESS if the example has completed successfully.
*		- XST_FAILURE if the example has failed.
*
* @note   	None
*
****************************************************************************/

/**
 * This table contains configuration information for each XADC Monitor/ADC
 * device in the system.
 */
XAdcPs_Config XAdcPs_ConfigTable[XPAR_XADCPS_NUM_INSTANCES] =
{
	{
		XPAR_XADCPS_0_DEVICE_ID,	/**< Unique ID of device */
		XPAR_XADCPS_0_BASEADDR		/**< Base address of device */
	}
};

XAdcPs_Config *XAdcPs_LookupConfig(u16 DeviceId)
{
	XAdcPs_Config *CfgPtr = NULL;
	u32 Index;

	for (Index=0; Index < 1; Index++) {
		if (XAdcPs_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAdcPs_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

/******************************************************************************/
int XAdcPs_SelfTest(XAdcPs *InstancePtr)
{
	int Status;
	u32 RegValue;

	/*
	 * Assert the argument
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);
	Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);


	/*
	 * Reset the device to get it back to its default state
	 */
	XAdcPs_Reset(InstancePtr);

	/*
	 * Write a value into the Alarm Threshold registers, read it back, and
	 * do the comparison
	 */
	XAdcPs_SetAlarmThreshold(InstancePtr, XADCPS_ATR_VCCINT_UPPER,
				  XADCPS_ATR_TEST_VALUE);
	RegValue = XAdcPs_GetAlarmThreshold(InstancePtr, XADCPS_ATR_VCCINT_UPPER);

	if (RegValue == XADCPS_ATR_TEST_VALUE) {
		Status = XST_SUCCESS;
	} else {
		Status = XST_FAILURE;
	}

	/*
	 * Reset the device again to its default state.
	 */
	XAdcPs_Reset(InstancePtr);
	/*
	 * Return the test result.
	 */
	return Status;
}

int XAdcPolledPrintfExample(u16 XAdcDeviceId)
{
	int Status;
	XAdcPs_Config *ConfigPtr;
	u32 TempRawData;
	u32 VccPintRawData;
	u32 VccPauxRawData;
	u32 VccPdroRawData;
	float TempData;
	float VccPintData;
	float VccPauxData;
	float MaxData;
	float MinData;
	XAdcPs *XAdcInstPtr = &XAdcInst;

	printf("\r\nDisplaying Xilinx onboard temperature readings:\r\n");

	/*
	 * Initialize the XAdc driver.
	 */
	ConfigPtr = XAdcPs_LookupConfig(XAdcDeviceId);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	XAdcPs_CfgInitialize(XAdcInstPtr, ConfigPtr,
				ConfigPtr->BaseAddress);

	/*
	 * Self Test the XADC/ADC device
	 */
	Status = XAdcPs_SelfTest(XAdcInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Disable the Channel Sequencer before configuring the Sequence
	 * registers.
	 */
	XAdcPs_SetSequencerMode(XAdcInstPtr, XADCPS_SEQ_MODE_SAFE);
	/*
	 * Read the on-chip Temperature Data (Current/Maximum/Minimum)
	 * from the ADC data registers.
	 */
	TempRawData = XAdcPs_GetAdcData(XAdcInstPtr, XADCPS_CH_TEMP);
	TempData = XAdcPs_RawToTemperature(TempRawData);
	printf("\r\nThe Current Temperature is %0d.%03d Centigrades.\r\n",
				(int)(TempData), XAdcFractionToInt(TempData));


	TempRawData = XAdcPs_GetMinMaxMeasurement(XAdcInstPtr, XADCPS_MAX_TEMP);
	MaxData = XAdcPs_RawToTemperature(TempRawData);
	printf("The Maximum Temperature is %0d.%03d Centigrades. \r\n",
				(int)(MaxData), XAdcFractionToInt(MaxData));

	TempRawData = XAdcPs_GetMinMaxMeasurement(XAdcInstPtr, XADCPS_MIN_TEMP);
	MinData = XAdcPs_RawToTemperature(TempRawData & 0xFFF0);
	printf("The Minimum Temperature is %0d.%03d Centigrades. \r\n",
				(int)(MinData), XAdcFractionToInt(MinData));

	printf("\r\n");

	return XST_SUCCESS;
}


/****************************************************************************/
/*
*
* This function converts the fraction part of the given floating point number
* (after the decimal point)to an integer.
*
* @param	FloatNum is the floating point number.
*
* @return	Integer number to a precision of 3 digits.
*
* @note
* This function is used in the printing of floating point data to a STDIO device
* using the xil_printf function. The xil_printf is a very small foot-print
* printf function and does not support the printing of floating point numbers.
*
*****************************************************************************/
int XAdcFractionToInt(float FloatNum)
{
	float Temp;

	Temp = FloatNum;
	if (FloatNum < 0) {
		Temp = -(FloatNum);
	}

	return( ((int)((Temp -(float)((int)Temp)) * (1000.0f))));
}


