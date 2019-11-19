#pragma once

/**
 * @file FrameManager.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Frame time checker
 */

/**
 * @brief Checks if frame time exceeded to allow for new frame
 * 
 */
class FrameManager
{
public:
	/**
	 * @brief Construct a new Frame Manager object
	 * 
	 * @param maxFrameRate 
	 */
	FrameManager(unsigned int maxFrameRate = 144);
	~FrameManager();
	/**
	 * @brief Call at the start of the frame to begin timing
	 * 
	 */
	void StartFrame();
	/**
	 * @brief Call at the end of the frame to wait until the min frame time is complete
	 * 
	 */
	void EndFrame();
	/**
	 * @brief Get the current time within frame
	 * 
	 * @return unsigned int 
	 */
	unsigned int GetFrameTime();
	/**
	 * @brief Get the current time within frame
	 * 
	 * @return float 
	 */
	float GetFloatFrameTime() const;
	/**
	 * @brief Set the Frame Rate
	 * 
	 * @param maxFrameRate 
	 */
	void SetFrameRate(unsigned int maxFrameRate);

private:
	unsigned int m_tick_start;
	unsigned int m_tick_end;
	unsigned int m_ticks_per_frame;
	unsigned int m_current_frame_time;
};

