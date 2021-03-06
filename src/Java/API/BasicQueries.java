package API;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

import static Properties.Read.readValue;

class BasicQueries {
    private static final String DB_DRIVER = "com.mysql.cj.jdbc.Driver";
    //"jdbc:mysql://localhost:3306/meterData"; // 192.168.43.150:3306/meterData
    private static final String DB_CONNECTION = "jdbc:mysql://" + readValue("IP_server") + ":"
            + readValue("Poort_DB") + "/" + readValue("Naam_DB");
    private static final String DB_USER = readValue("Usr_DB"); //        remote: "BewustE"                locale: "root"
    private static final String DB_PASSWORD = readValue("Ww_DB"); //        remote: "yoloww" OR "raspberry"    locale: ""

    private BasicQueries() {
    }

    /**
     * Gets db connection.
     *
     * @return the db connection
     */
    static Connection getDbConnection() throws SQLException {
        Connection dbConnection = null;
        try {
            Class.forName(DB_DRIVER);
            dbConnection = DriverManager.getConnection(DB_CONNECTION, DB_USER, DB_PASSWORD);
            return dbConnection;
        } catch (ClassNotFoundException e) {
            System.out.println(e.getMessage());
            e.printStackTrace();
        }
        return dbConnection;
    }

    /**
     * Close con.
     *
     * @param conn the conn
     */
    static void closeCon(Connection conn) {
        if (conn != null) {
            try {
                conn.close();
            } catch (SQLException e) {
                e.getMessage();
            }
        }
    }

}

