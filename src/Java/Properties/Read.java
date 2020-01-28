package Properties;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.Set;

public class Read {
    private static final String propertiesLocatie = System.getProperty("user.dir") + "/resources/instellingen.properties";

    private Read() {

    }

    public static String readValue(String item) {
        try (InputStream input = new FileInputStream(propertiesLocatie)) {
            Properties prop = new Properties();
            prop.load(input);
            return prop.getProperty(item);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    static Set<String> getAllProperties() {
        try (InputStream input = new FileInputStream(propertiesLocatie)){
            Properties prop = new Properties();
            prop.load(input);
            input.close();
            System.out.println(prop);
            return prop.stringPropertyNames();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}