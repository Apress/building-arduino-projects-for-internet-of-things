/***************************************************************************
 * File Name:   MQTTClient.java
 * Description: This class connects to MQTT and subscribes to a topic
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 05 - IoT Patterns: Relatime Clients
 * Website:     http://codifythings.com
 **************************************************************************/

package com.codifythings.intrusiondetectionsystem;

import android.util.Log;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.text.DateFormat;
import java.util.Date;

public class MQTTClient {

    private static final String TAG = "MQTTClient";
    private String mqttBroker = "tcp://iot.eclipse.org:1883";
    private String mqttTopic = "codifythings/intrusiondetection";
    private String deviceId = "androidClient";

    // Variables to store reference to the user interface activity.
    private MainActivity activity = null;

    public MQTTClient(MainActivity activity) {
        this.activity = activity;
    }

    public void connectToMQTT() throws MqttException {
        // Request clean session in the connection options.
        Log.i(TAG, "Setting Connection Options");
        MqttConnectOptions options = new MqttConnectOptions();
        options.setCleanSession(true);

        // Attempt a connection to MQTT broker using the values of connection variables.
        Log.i(TAG, "Creating New Client");
        MqttClient client = new MqttClient(mqttBroker, deviceId, new MemoryPersistence());
        client.connect(options);

        // Set callback method name that will be invoked when a new message is posted to topic,
        // MqttEventCallback class is defined later in the code.
        Log.i(TAG, "Subscribing to Topic");
        client.setCallback(new MqttEventCallback());

        // Subscribe to topic "codifythings/intrusiondetection", whenever a new message is published to
        // this topic MqttEventCallback.messageArrived will be called.
        client.subscribe(mqttTopic, 0);
    }

    // Implementation of the MqttCallback.messageArrived method, which is invoked whenever a
    // new message is published to the topic “codifythings/intrusiondetection”.
    private class MqttEventCallback implements MqttCallback {
        @Override
        public void connectionLost(Throwable arg0) {
            // Do nothing
        }

        @Override
        public void deliveryComplete(IMqttDeliveryToken arg0) {
            // Do nothing
        }

        @Override
        public void messageArrived(String topic, final MqttMessage msg) throws Exception {
            Log.i(TAG, "New Message Arrived from Topic - " + topic);

            try {
                // Append the payload message "Intrusion Detected" with "@ Current Time".
                DateFormat df = DateFormat.getDateTimeInstance();
                String sensorMessage = new String(msg.getPayload()) + " @ " +
                        df.format(new Date());

                // User is not going to be on the screen all the time, so create a notification.
                activity.createNotification("Intrusion Detection System", sensorMessage);

                // Update the screen with newly received message.
                activity.updateView(sensorMessage);
            } catch (Exception ex) {
                Log.e(TAG, ex.getMessage());
            }
        }
    }
}
