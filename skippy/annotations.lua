---@diagnostic disable: missing-return
-- luacheck: ignore


---@class skippy
local skippy = {}

--- Start the sensor listeners.
function skippy.start() end

--- Stop the sensor listeners.
function skippy.stop() end

--- All the steps since the device was powered on.
--- For Android this means the TYPE_STEP_COUNTER sensor data.
--- @return integer
function skippy.get_steps() end

--- Amount of steps detected since the app was started.
--- For Android this means the TYPE_STEP_DETECTOR sensor data.
--- @return integer
function skippy.get_steps_detector() end
