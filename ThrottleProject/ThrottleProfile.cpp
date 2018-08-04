#include "stdafx.h"
#include "ThrottleProfile.h"


ThrottleProfile::ThrottleProfile(long lThrottleBucket, long lBucketExpiry, long lTimeDelay):m_lThrottleBucket(lThrottleBucket), m_lBucketExpiry(lBucketExpiry), m_lAvailableSlots(m_lThrottleBucket), m_lTimeDelay(lTimeDelay)
{
	m_tBucketStart = std::chrono::high_resolution_clock::now();
	m_tLastFill = std::chrono::high_resolution_clock::now();
}


ThrottleProfile::~ThrottleProfile()
{
}

bool ThrottleProfile::reset() {
	m_lAvailableSlots = m_lThrottleBucket;
	m_tBucketStart = std::chrono::high_resolution_clock::now();
	return true;
}

bool ThrottleProfile::isThrottleExpired() {
	auto curTime = std::chrono::high_resolution_clock::now();
	auto TimeElapsed = curTime - m_tBucketStart;
	long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(TimeElapsed).count();

	if (duration >= m_lBucketExpiry)
		return true;

	return false;
}

void ThrottleProfile::updateBucketTimeAndSlots() {
	if (!isThrottleExpired())
		return;

	reset();
}

bool ThrottleProfile::utilizeThrottle() {
	updateBucketTimeAndSlots();
	if (m_lAvailableSlots > 0) {
		m_lAvailableSlots--;
		m_tLastFill = std::chrono::high_resolution_clock::now();
		return true;
	}
	return false;
}