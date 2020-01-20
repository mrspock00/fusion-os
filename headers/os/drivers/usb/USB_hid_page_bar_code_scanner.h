/*
 * Copyright 2004-2010, Haiku Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _USB_HID_PAGE_BAR_CODE_SCANNER_H
#define _USB_HID_PAGE_BAR_CODE_SCANNER_H


/* Reference:
 *		HID Usage Page 0x8C: BAR CODE SCANNER
 *		HID Point of Sale Usage Tables Ver. 1.0
 *		http://www.usb.org/developers/devclass_docs/pos1_02.pdf
 */

// Usage IDs
enum {
	B_HID_UID_BCS_BAR_CODE_BADGE_READER = 0x01,
	B_HID_UID_BCS_BAR_CODE_SCANNER,
	B_HID_UID_BCS_DUMB_BAR_CODE_SCANNER,
	B_HID_UID_BCS_CORDLESS_SCANNER_BASE,
	B_HID_UID_BCS_BAR_CODE_SCANNER_CRADLE,
	
	B_HID_UID_BCS_ATTRIBUTE_REPORT = 0x10,
	B_HID_UID_BCS_SETTINGS_REPORT,
	B_HID_UID_BCS_SCANNED_DATA_REPORT,
	B_HID_UID_BCS_RAW_SCANNED_DATA_REPORT,
	B_HID_UID_BCS_TRIGGER_REPORT,
	B_HID_UID_BCS_STATUS_REPORT,
	B_HID_UID_BCS_UPC_EAN_CONTROL_REPORT,
	B_HID_UID_BCS_EAN_2_3_LABEL_CONTROL_REPORT,
	B_HID_UID_BCS_CODE_39_CONTROL_REPORT,
	B_HID_UID_BCS_INTERLEAVED_2_OF_5_CONTROL_REPORT,
	B_HID_UID_BCS_STANDARD_2_OF_5_CONTROL_REPORT,
	B_HID_UID_BCS_MSI_PLESSEY_CONTROL_REPORT,
	B_HID_UID_BCS_CODABAR_CONTROL_REPORT,
	B_HID_UID_BCS_CODE_128_CONTROL_REPORT,
	B_HID_UID_BCS_MISC_1D_CONTROL_REPORT,
	B_HID_UID_BCS_2D_CONTROL_REPORT,
	
	B_HID_UID_BCS_AIMING_POINTER_MODE = 0x30,
	B_HID_UID_BCS_BAR_CODE_PRESENT_SENSOR,
	B_HID_UID_BCS_CLASS_1A_LASER,
	B_HID_UID_BCS_CLASS_2_LASER,
	B_HID_UID_BCS_HEATER_PRESENT,
	B_HID_UID_BCS_CONTACT_SCANNER,
	B_HID_UID_BCS_ELECTRONIC_ARTICLE_SURVEILLANCE_NOTIFICATION,
	B_HID_UID_BCS_CONSTANT_ELECTRONIC_ARTICLE_SURVEILLANCE,
	B_HID_UID_BCS_ERROR_INDICATION,
	B_HID_UID_BCS_FIXED_BEEPER,
	B_HID_UID_BCS_GOOD_DECODE_INDICATION,
	B_HID_UID_BCS_HANDS_FREE_SCANNING,
	B_HID_UID_BCS_INTRINSICALLY_SAFE,
	B_HID_UID_BCS_KLASSE_EINS_LASER,
	B_HID_UID_BCS_LONG_RANGE_SCANNER,
	B_HID_UID_BCS_MIRROR_SPEED_CONTROL,
	B_HID_UID_BCS_NOT_ON_FILE_INDICATION,
	B_HID_UID_BCS_PROGRAMMABLE_BEEPER,
	B_HID_UID_BCS_TRIGGERLESS,
	B_HID_UID_BCS_WAND,
	B_HID_UID_BCS_WATER_RESISTANT,
	B_HID_UID_BCS_MULTI_RANGE_SCANNER,
	B_HID_UID_BCS_PROXIMITY_SENSOR,
	
	B_HID_UID_BCS_FRAGMENT_DECODING = 0x4d,
	B_HID_UID_BCS_SCANNER_READ_CONFIDENCE,
	B_HID_UID_BCS_DATA_PREFIX,
	B_HID_UID_BCS_PREFIX_AIMI,
	B_HID_UID_BCS_PREFIX_NONE,
	B_HID_UID_BCS_PREFIX_PROPRIETARY,
	
	B_HID_UID_BCS_ACTIVE_TIME = 0x55,
	B_HID_UID_BCS_AIMING_LASER_PATTERN,
	B_HID_UID_BCS_BAR_CODE_PRESENT,
	B_HID_UID_BCS_BEEPER_STATE,
	B_HID_UID_BCS_LASER_ON_TIME,
	B_HID_UID_BCS_LASER_STATE,
	B_HID_UID_BCS_LOCKOUT_TIME,
	B_HID_UID_BCS_MOTOR_STATE,
	B_HID_UID_BCS_MOTOR_TIMEOUT,
	B_HID_UID_BCS_POWER_ON_RESET_SCANNER,
	B_HID_UID_BCS_PREVENT_READ_OF_BARCODES,
	B_HID_UID_BCS_INITIATE_BARCODE_READ,
	B_HID_UID_BCS_TRIGGER_STATE,
	B_HID_UID_BCS_TRIGGER_MODE,
	B_HID_UID_BCS_TRIGGER_MODE_BLINKING_LASER_ON,
	B_HID_UID_BCS_TRIGGER_MODE_CONTINUOUS_LASER_ON,
	B_HID_UID_BCS_TRIGGER_MODE_LASER_ON_WHILE_PULLED,
	B_HID_UID_BCS_TRIGGER_MODE_LASER_STAYS_ON_AFTER_TRIGGER_RELEASE,
	
	B_HID_UID_BCS_COMMIT_PARAMETERS_TO_NVM = 0x6d,
	B_HID_UID_BCS_PARAMETER_SCANNING,
	B_HID_UID_BCS_PARAMETERS_CHANGED,
	B_HID_UID_BCS_SET_PARAMETER_DEFAULT_VALUES,
	
	B_HID_UID_BCS_SCANNER_IN_CRADLE = 0x75,
	B_HID_UID_BCS_SCANNER_IN_RANGE,
	
	B_HID_UID_BCS_AIM_DURATION = 0x7a,
	B_HID_UID_BCS_GOOD_READ_LAMP_DURATION,
	B_HID_UID_BCS_GOOD_READ_LAMP_INTENSITY,
	B_HID_UID_BCS_GOOD_READ_LED,
	B_HID_UID_BCS_GOOD_READ_TONE_FREQUENCY,
	B_HID_UID_BCS_GOOD_READ_TONE_LENGTH,
	B_HID_UID_BCS_GOOD_READ_TONE_VOLUME,
	
	B_HID_UID_BCS_NO_READ_MESSAGE = 0x82,
	B_HID_UID_BCS_NOT_ON_FILE_VOLUME,
	B_HID_UID_BCS_POWERUP_BEEP,
	B_HID_UID_BCS_SOUND_ERROR_BEEP,
	B_HID_UID_BCS_SOUND_NOT_ON_FILE_BEEP,
	B_HID_UID_BCS_GOOD_READ_WHEN_TO_WRITE,
	B_HID_UID_BCS_GRWTI_AFTER_DECODE,
	B_HID_UID_BCS_GRWTI_BEEP_LAMP_AFTER_TRANSMIT,
	B_HID_UID_BCS_GRWTI_NO_BEEP_LAMP_USE_AT_ALL,
	
	B_HID_UID_BCS_BOOKLAND_EAN = 0x91,
	B_HID_UID_BCS_CONVERT_EAN_8_TO_13_TYPE,
	B_HID_UID_BCS_CONVERT_UPC_A_TO_EAN_13,
	B_HID_UID_BCS_CONVERT_UPC_E_TO_A,
	B_HID_UID_BCS_EAN_13,
	B_HID_UID_BCS_EAN_8,
	B_HID_UID_BCS_EAN_99_128_MANDATORY,
	B_HID_UID_BCS_EAN_99_P5_128_OPTIONAL,
	
	B_HID_UID_BCS_UPC_EAN = 0x9a,
	B_HID_UID_BCS_UPC_EAN_COUPON_CODE,
	B_HID_UID_BCS_UPC_EAN_PERIODICALS,
	B_HID_UID_BCS_UPC_A,
	B_HID_UID_BCS_UPC_A_WITH_128_MANDATORY,
	B_HID_UID_BCS_UPC_A_WITH_128_OPTIONAL,
	B_HID_UID_BCS_UPC_A_WITH_P5_OPTIONAL,
	B_HID_UID_BCS_UPC_E,
	B_HID_UID_BCS_UPC_E1,
	
	B_HID_UID_BCS_PERIODICAL = 0xa9,
	B_HID_UID_BCS_PERIODICAL_AUTO_DISCRIMINATE_PLUS_2,
	B_HID_UID_BCS_PERIODICAL_ONLY_DECODE_WITH_PLUS_2,
	B_HID_UID_BCS_PERIODICAL_IGNORE_PLUS_2,
	B_HID_UID_BCS_PERIODICAL_AUTO_DISCRIMINATE_PLUS_5,
	B_HID_UID_BCS_PERIODICAL_ONLY_DECODE_WITH_PLUS_5,
	B_HID_UID_BCS_PERIODICAL_IGNORE_PLUS_5,
	B_HID_UID_BCS_CHECK,
	B_HID_UID_BCS_CHECK_DISABLE_PRICE,
	B_HID_UID_BCS_CHECK_ENABLE_4_DIGIT_PRICE,
	B_HID_UID_BCS_CHECK_ENABLE_5_DIGIT_PRICE,
	B_HID_UID_BCS_CHECK_ENABLE_EUROPEAN_4_DIGIT_PRICE,
	B_HID_UID_BCS_CHECK_ENABLE_EUROPEAN_5_DIGIT_PRICE,
	
	B_HID_UID_BCS_EAN_TWO_LABEL = 0xb7,
	B_HID_UID_BCS_EAN_THREE_LABEL,
	B_HID_UID_BCS_EAN_8_FLAG_DIGIT_1,
	B_HID_UID_BCS_EAN_8_FLAG_DIGIT_2,
	B_HID_UID_BCS_EAN_8_FLAG_DIGIT_3,
	B_HID_UID_BCS_EAN_13_FLAG_DIGIT_1,
	B_HID_UID_BCS_EAN_13_FLAG_DIGIT_2,
	B_HID_UID_BCS_EAN_13_FLAG_DIGIT_3,			
	B_HID_UID_BCS_ADD_EAN_2_3_LABEL_DEFINITION,
	B_HID_UID_BCS_CLEAR_ALL_EAN_2_3_LABEL_DEFINITIONS,
	
	B_HID_UID_BCS_CODABAR = 0xc3,
	B_HID_UID_BCS_CODE_128,
	
	B_HID_UID_BCS_CODE_39 = 0xc7,
	B_HID_UID_BCS_CODE_93,
	B_HID_UID_BCS_FULL_ASCII_CONVERSION,
	B_HID_UID_BCS_INTERLEAVED_2_OF_5,
	B_HID_UID_BCS_ITALIAN_PHARMACY_CODE,
	B_HID_UID_BCS_MSI_PLESSEY,
	B_HID_UID_BCS_STANDARD_2_OF_5_IATA,
	B_HID_UID_BCS_STANDARD_2_OF_5,
	
	B_HID_UID_BCS_TRANSMIT_START_STOP = 0xd3,
	B_HID_UID_BCS_TRI_OPTIC,
	B_HID_UID_BCS_UCC_EAN_128,
	B_HID_UID_BCS_CHECK_DIGIT,
	B_HID_UID_BCS_CHECK_DIGIT_DISABLE,
	B_HID_UID_BCS_CHECK_DIGIT_ENABLE_INTERLEAVED_2_OF_5_OPCC,
	B_HID_UID_BCS_CHECK_DIGIT_ENABLE_INTERLEAVED_2_OF_5_USS,
	B_HID_UID_BCS_CHECK_DIGIT_ENABLE_STANDARD_2_OF_5_OPCC,
	B_HID_UID_BCS_CHECK_DIGIT_ENABLE_STANDARD_2_OF_5_USS,
	B_HID_UID_BCS_CHECK_DIGIT_ENABLE_ONE_MSI_PLESSEY,
	B_HID_UID_BCS_CHECK_DIGIT_ENABLE_TWO_MSI_PLESSEY,
	B_HID_UID_BCS_CHECK_DIGIT_CODABAR_ENABLE,
	B_HID_UID_BCS_CHECK_DIGIT_CODE_39_ENABLE,
	
	B_HID_UID_BCS_DISABLE_CHECK_DIGIT_TRANSMIT = 0xf1,
	B_HID_UID_BCS_ENABLE_CHECK_DIGIT_TRANSMIT,
	
	B_HID_UID_BCS_SYMBOLOGY_IDENTIFIER_1 = 0xfb,
	B_HID_UID_BCS_SYMBOLOGY_IDENTIFIER_2,
	B_HID_UID_BCS_SYMBOLOGY_IDENTIFIER_3,
	B_HID_UID_BCS_DECODED_DATA,
	B_HID_UID_BCS_DECODE_DATA_CONTINUED,
	B_HID_UID_BCS_BAR_SPACE_DATA,
	B_HID_UID_BCS_SCANNER_DATA_ACCURACY,
	B_HID_UID_BCS_RAW_DATA_POLARITY,
	B_HID_UID_BCS_POLARITY_INVERT_BAR_CODE,
	B_HID_UID_BCS_POLARITY_NORMAL_BAR_CODE,
	
	B_HID_UID_BCS_MINIMUM_LENGTH_TO_DECODE = 0x106,
	B_HID_UID_BCS_MAXIMUM_LENGTH_TO_DECODE,
	B_HID_UID_BCS_FIRST_DISCRETE_LENGTH_TO_DECODE,
	B_HID_UID_BCS_SECOND_DESCRETE_LENGTH_TO_DECODE,
	B_HID_UID_BCS_DATA_LENGTH_METHOD,
	B_HID_UID_BCS_DL_METHOD_READ_ANY,
	B_HID_UID_BCS_DL_METHOD_CHECK_IN_RANGE,
	B_HID_UID_BCS_DL_METHOD_CHECK_FOR_DISCRETE,
	
	B_HID_UID_BCS_AZTEC_CODE = 0x110,
	B_HID_UID_BCS_BC412,
	B_HID_UID_BCS_CHANNEL_CODE,
	B_HID_UID_BCS_CODE_16,
	B_HID_UID_BCS_CODE_32,
	B_HID_UID_BCS_CODE_49,
	B_HID_UID_BCS_CODE_ONE,
	B_HID_UID_BCS_COLORCODE,
	B_HID_UID_BCS_DATA_MATRIX,
	B_HID_UID_BCS_MAXI_CODE,
	B_HID_UID_BCS_MICRO_PDF,
	B_HID_UID_BCS_PDF_417,
	B_HID_UID_BCS_POSI_CODE,
	B_HID_UID_BCS_QR_CODE,
	B_HID_UID_BCS_SUPER_CODE,
	B_HID_UID_BCS_ULTRA_CODE,
	B_HID_UID_BCS_USD_5_SLUG_CODE,
	B_HID_UID_BCS_VERI_CODE
};


#endif // _USB_HID_PAGE_BAR_CODE_SCANNER_H
