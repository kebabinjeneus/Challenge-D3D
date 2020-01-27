package Properties;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Properties;
import java.util.Set;

import static Properties.Read.getAllProperties;
import static Properties.Read.readValue;

public class Store {
    private static final String propertiesLocatie = System.getProperty("user.dir") + "/resources/instellingen.properties";

    private Store() {

    }

    public static void storeValue(String item, String value) {
        Properties prop = new Properties();
        Set<String> allProperties = getAllProperties();
        if (allProperties != null) {
            for (String i : allProperties) {
                prop.setProperty(i, readValue(i));
            }
        }
        try (OutputStream output = new FileOutputStream(propertiesLocatie)) {
            prop.setProperty(item, value);

            prop.store(output, null);
            output.close();

//            System.out.println(prop);
        } catch (IOException io) {
            io.printStackTrace();
        }
    }

    public static void initialiseerProperties() {
        // General
        storeValue("IP_server", "192.168.43.150");

        // Database
        storeValue("Poort_DB", "3306");
        storeValue("Naam_DB", "BewustE");

        // Local files
        // TODO change to proper location on pi -> /home/pi/p1.data <- oid
        storeValue("Locatie_P1_mock", "\\resources\\p1.data");

    }
}