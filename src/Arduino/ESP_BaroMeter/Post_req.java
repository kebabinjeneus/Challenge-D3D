package sample;

import java.net.*;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.io.StringWriter;

public class Post_req {

    private String baseUrl;

    public RestHandler(String baseUrl) {
        this.baseUrl = baseUrl;
    }

    public String Post(String route, String jsonString) {
        try {
            byte[] out = jsonString.getBytes(StandardCharsets.UTF_8);
            int length = out.length;
            URL url = new URL(this.baseUrl + route);
            URLConnection con = url.openConnection();
            HttpURLConnection http = (HttpURLConnection)con;

            http.setRequestMethod("POST");  // PUT is another valid option
            http.setDoOutput(true);
            http.setFixedLengthStreamingMode(length);
            http.setRequestProperty("Content-Type", "application/json; charset=UTF-8");
            http.setRequestProperty("User-Agent","Mozilla/5.0 ( compatible ) ");
            http.setRequestProperty("Accept", "*/*");
            http.connect();

            try (OutputStream os = http.getOutputStream()) {
                os.write(out);
            }

            try (InputStream os = http.getInputStream()) {
                java.util.Scanner s = new java.util.Scanner(os).useDelimiter("\\A");
                String i = s.hasNext() ? s.next() : "";
                return i;
            }
        }
        catch (Exception e) {
            System.out.println(e);
        }

        return "";
        // Do something with http.getInputStream()
    }
}