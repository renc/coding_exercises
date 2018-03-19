
#include <string> // for std::to_string 

enum ReturnType {
	Success = 0, 
	FailToOpenChannel,
	FailOfTimeOut, 
	FailOfGetNextData, 
	FailOfUnknownReason 
};


// Usage: 
//   iReturnCode = tryToGetValue(...);
//   check the iReturnCode 
int tryToGetValue(
	unsigned int iTimeOutSecond,
	std::string &sPhysicallyValue,
	std::string &sErrorMessage)
{
	// open a channel
	int iChannelHandle = open();

	if (iChannelHandle < 0)
	{
		// negative error code, fail to open the channel
		// to log some information
		sErrorMessage = "Fail to open the channel";
		log(sErrorMessage);
		return ReturnType::FailToOpenChannel;
	}
	else
	{
		// succeed to open the channel. 
		
		const float fBandLowerLimit = -1.0f;
		const float fBandUpperLimit = 1.8f;
		
		const int iStartTime = getCurrentTime();
		while (getCurrentTime() - iStartTime < iTimeOutSecond)
		{
			std::string sDataBuffer;
			int iReturnCode = getNextData(iChannelHandle, sDataBuffer);
			if (iReturnCode < 0) 
			{
				sErrorMessage = "Fail to get the next data";
				return ReturnType::FailOfGetNextData;
			}

			float fDegreeValue = 10;// which is outside the tolerance band
			
			// the data buffer is valid, 
			if (sDataBuffer != nullptr) // pointer is not null
			{
				// to get the measure value from the data buffer 
				char cMSB = get byte 4 from sDataBuffer;
				char cLSB = get byte 3 from sDataBuffer;
				unsigned int iMeasureValue = toInt(cMSB) << 16 + toInt(cLSB); // suppose the unsigned int here is 32bits.

				// to calculate from meansure value to physically value
				// !! i donot understand what this mean:
				// 0.00289 [Grad]/Bit, with an offset of -40 degree ??
				??
			}
			else {
				sErrorMessage = "Fail to get the next data";
				return ReturnType::FailOfGetNextData;
			}


			// 
			if (fBandLowerLimit < fDegreeValue && fDegreeValue < fBandUpperLimit)
			{
				sPhysicallyValue = std::to_string(fDegreeValue);

				// before existing the program, close the channel.
				close(iChannelHandle);
				return ReturnType::Success;
			}

			// keep looping 
		}

		// before existing the program, close the channel.
		close(iChannelHandle);
		return ReturnType::FailOfTimeOut;
	}
	return ReturnType::FailOfUnknownReason;
}
 