#ifndef THROTTLE_PROFILE
#define THROTTLE_PROFILE
#include <iostream>
#include <chrono>

using namespace std::chrono;

class ThrottleProfile
{
public:
	ThrottleProfile(long lThrottleBucket, long lBucketExpiry, long lTimeDelay);
	virtual ~ThrottleProfile();

	inline long getThrottleBucket() { return m_lThrottleBucket; }
	inline long getThrottleExpiry() { return m_lBucketExpiry; }
	inline long getAvailableSlots() { return m_lAvailableSlots; }
	inline long getTimeDelay() { return m_lTimeDelay; }

	bool isThrottleExpired();
	bool utilizeThrottle();

private:
	ThrottleProfile(ThrottleProfile& handle) = delete;
	ThrottleProfile(ThrottleProfile&& handle) = delete;
	ThrottleProfile& operator=(const ThrottleProfile& other) = delete;
	ThrottleProfile& operator=(ThrottleProfile&& other) = delete;

	void updateBucketTimeAndSlots();
	bool reset();

	long m_lThrottleBucket;// This is total number of messages allowed in Bucket e.g. 100
	long m_lBucketExpiry; // 1000 milliseconds will be total time for one bucket.
	std::chrono::high_resolution_clock::time_point m_tBucketStart;// This is start time for sliding window bucket
	long m_lAvailableSlots;// Number of slots remaining from Throttle Bucket as of now.
	long m_lTimeDelay;// Time Delay to be introduced between two messages in milliseconds
	std::chrono::high_resolution_clock::time_point m_tLastFill;
};

#endif // !THROTTLE_PROFILE


