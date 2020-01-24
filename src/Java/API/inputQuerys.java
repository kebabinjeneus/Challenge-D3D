package API;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;

import static API.BasicQueries.closeCon;

public class inputQuerys {
    /**
     * @param dataPoint het huidige datapunt dat in de database geyeet dient te worden.
     * @throws SQLException
     */
    public static void dbFiller(meterData dataPoint) {
        Connection conn = null;
        try {
            conn = BasicQueries.getDbConnection();
            if(conn == null) {
                System.out.println("conn: null");
                return;
            }
            String query = "INSERT INTO verbruik (huidigVerbruik, totVerbruikLaag, totVerbruikHoog, gasverbruik)" +
                    "values (?, ?, ?, ?);";
            PreparedStatement preparedStmt = conn.prepareStatement(query);
            preparedStmt.setDouble(1, dataPoint.getHuidigVerbruik());
            preparedStmt.setDouble(2, dataPoint.getTotVerbruikLaag());
            preparedStmt.setDouble(3, dataPoint.getTotVerbruikHoog());
            preparedStmt.setDouble(4, dataPoint.getGasVerbruik());

            preparedStmt.execute();
            conn.close();
            System.out.println("Point written to database");
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            closeCon(conn);
        }
    }
}
