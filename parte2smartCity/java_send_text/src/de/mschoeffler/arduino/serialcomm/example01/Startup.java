package de.mschoeffler.arduino.serialcomm.example01;

import java.io.IOException;


import com.fazecast.jSerialComm.SerialPort;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Scanner;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

/**
 * Simple application that is part of an tutorial. 
 * The tutorial shows how to establish a serial connection between a Java and Arduino program.
 * @author Michael Schoeffler (www.mschoeffler.de)
 *
 */
public class Startup {

	public static void main(String[] args) throws IOException, InterruptedException {
		SerialPort sp = SerialPort.getCommPort("COM6"); // device name TODO: must be changed
		sp.setComPortParameters(9600, 8, 1, 0); // default connection settings for Arduino
		sp.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0, 0); // block until bytes can be written
		
		if (sp.openPort()) {
			System.out.println("Port is open :)");
		} else {
			System.out.println("Failed to open port :(");
			return;
		}		
		
		while(true) {
			Integer currentWeather= getWeatherCode();
			sp.getOutputStream().write(currentWeather.byteValue());
			sp.getOutputStream().flush();
			System.out.println("Sent currentWeather code: " + currentWeather);

			Thread.sleep(40000);
		}		
		
		/* if (sp.closePort()) {
			System.out.println("Port is closed :)");
		} else {
			System.out.println("Failed to close port :(");
			return;
		} */
		

	}

	public static Integer getWeatherCode(){//  consumo de https://medium.com/swlh/getting-json-data-from-a-restful-api-using-java-b327aafb3751
		try {

			URL url = new URL("https://api.open-meteo.com/v1/forecast?latitude=6.199656063350807&longitude=-75.5792788322791&current_weather=true");

			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setRequestMethod("GET");
			conn.connect();

			//Getting the response code
			int responsecode = conn.getResponseCode();

			if (responsecode != 200) {
				throw new RuntimeException("HttpResponseCode: " + responsecode);
			} else {

				String inline = "";
				Scanner scanner = new Scanner(url.openStream());

				//Write all the JSON data into a string using a scanner
				while (scanner.hasNext()) {
					inline += scanner.nextLine();
				}

				//Close the scanner
				scanner.close();

				//Using the JSON simple library parse the string into a json object
				JSONParser parse = new JSONParser();
				JSONObject data_obj = (JSONObject) parse.parse(inline);

				//Get the required object from the above created object
				JSONObject obj = (JSONObject) data_obj.get("current_weather");
				System.out.println(obj.get("weathercode"));
				if(((Long)obj.get("weathercode"))>=51L){//https://open-meteo.com/en/docs
					return 1;
				}
				return 0;

			}

		} catch (Exception e) {
			System.out.println("el llamado a la api fallo");
			return 0;
		}
	}

}
