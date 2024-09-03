package com.naakkadev.skippy;

import android.hardware.SensorManager;
import android.hardware.SensorEventListener;
import android.hardware.SensorEvent;
import android.hardware.Sensor;
import android.content.Context;
import java.lang.reflect.Array;
import android.content.pm.PackageManager;
import android.os.Build;

import android.util.Log;

class SkippyExtension implements SensorEventListener {
    private static final String TAG = "defold";

    private SensorManager sensorManager;
    private Sensor senStepCounter;
    private Sensor senStepDetector;

    private static int stepsTaken = 0;
    private static int stepDetector = 0;

    private static SkippyExtension instance = null;

    public static SkippyExtension getInstance() {
        if (instance == null) {
            instance = new SkippyExtension();
        }
        return instance;
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        switch (event.sensor.getType()) {
            case Sensor.TYPE_STEP_COUNTER:
                stepsTaken = (int) event.values[0];
                break;
            case Sensor.TYPE_STEP_DETECTOR:
                stepDetector++;
                break;
        }
    }

    // Method to check that the running device has the required capability to
    // perform step detection.
    public boolean HasGotSensorCaps(Context context) {
        PackageManager pm = context.getPackageManager();

        // Require at least Android KitKat
        int currentApiVersion = Build.VERSION.SDK_INT;

        // Check that the device supports the step counter and detector sensors
        return currentApiVersion >= 19
                && pm.hasSystemFeature(PackageManager.FEATURE_SENSOR_STEP_COUNTER)
                && pm.hasSystemFeature(PackageManager.FEATURE_SENSOR_STEP_DETECTOR);
    }

    private void registerSensors(Context context) {
        // Double check that the device has the required sensor capabilities
        if (!HasGotSensorCaps(context)) {
            // Somehow tell user that their device does not support these
            return;
        }

        // Register the listeners. Used for receiving notifications from
        // the SensorManager when sensor values have changed.
        sensorManager.registerListener(this, senStepCounter, SensorManager.SENSOR_DELAY_NORMAL);
        sensorManager.registerListener(this, senStepDetector, SensorManager.SENSOR_DELAY_NORMAL);
    }

    // Simple function that un-registers all of the required sensor listeners
    // from the sensor manager.
    private void unRegisterSensors(Context context) {
        // Double check that the device has the required sensor capabilities
        // If not then we can simply return as nothing will have been already
        // registered
        if (!HasGotSensorCaps(context)) {
            return;
        }

        // Perform un-registration of the sensor listeners
        sensorManager.unregisterListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_STEP_COUNTER));
        sensorManager.unregisterListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_STEP_DETECTOR));
    }

    private void start(Context context) {
        // Assign the sensor manager
        sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        // Assign the sensors
        senStepCounter = sensorManager.getDefaultSensor(Sensor.TYPE_STEP_COUNTER);
        senStepDetector = sensorManager.getDefaultSensor(Sensor.TYPE_STEP_DETECTOR);

        // Spew out warnings if any of the sensors was not found
        if (senStepCounter == null) {
            Log.w(TAG, "Unable to find a step counter sensor.");
        }
        if (senStepDetector == null) {
            Log.w(TAG, "Unable to find a step detector sensor.");
        }

        registerSensors(context);
    }

    private void stop(Context context) {
        unRegisterSensors(context);
    }

    // Public methods
    public static void startStep(Context context) {
        getInstance().start(context);
    }

    public static void stopStep(Context context) {
        getInstance().stop(context);
    }

    public static int getSteps() {
        return stepsTaken;
    }

    public static int getStepDetector() {
        return stepDetector;
    }
}
