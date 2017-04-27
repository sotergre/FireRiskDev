package org.conservation.ci.cabs.firerisk.data;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStream;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import org.conservation.ci.cabs.firerisk.exceptions.FTPException;
import org.conservation.ci.cabs.firerisk.exceptions.FireRiskException;
import org.conservation.ci.cabs.firerisk.util.FtpHelper;

public class MOD7Data
{
//    private final String TWO_DIGITS_REPLACEMENT_REG_EX="^\\d\\d$";
//    private final String TWO_DIGITS_PREFIX="0";
//    private final String ONE_DIGIT_REPLACEMENT_REG_EX="^\\d$";
//    private final String ONE_DIGIT_PREFIX="00";

    private final String MOD7_FTP_SITE = "ladsweb.nascom.nasa.gov";
    private final String META_DATA_DIRECTORY = "geoMeta/6/TERRA/";
//    private final String MOD7_DATA_DIRECTORY = "allData/6/MOD07_L2";
    private final String GEO_META_FILENAME_PREFIX = "MOD03_";
    public final String GEO_META_FILENAME_DATE_FORMAT = "yyyy-MM-dd";
    private final String GEO_META_FILENAME_SUFFIX = ".txt";
    private final String TEMP_FILENAME = "tempGeoMeta.dat";

    private final String COMMENT_PREFIX = "#";
    private final String META_DATA_DELIMITER = ",";
    private final String DATA_FILE_DELIMITER = "[.]";
    private final String FILENAME_SEPARATOR = ".";

    private final String DATA_FILE_PREFIX = "MOD07_L2";
    private final String NEW_LINE = "\n";

    private final int FILENAME_INDEX = 0;
    private final int DAY_NIGHT_INDEX = 4;
    private final int GRING_LONG_1_INDEX = 9;
    private final int GRING_LONG_2_INDEX = 10;
    private final int GRING_LONG_3_INDEX = 11;
    private final int GRING_LONG_4_INDEX = 12;
    private final int GRING_LAT_1_INDEX = 13;
    private final int GRING_LAT_2_INDEX = 14;
    private final int GRING_LAT_3_INDEX = 15;
    private final int GRING_LAT_4_INDEX = 16;

    private final String DAY_VALUE = "D";

    private BufferedReader metaDataReader;
    private boolean metaDataDownloaded;
    private String currentYear;

    public MOD7Data() throws FireRiskException
    {
        metaDataDownloaded = false;
        currentYear = String.valueOf(Calendar.getInstance().get(Calendar.YEAR));
    }

    public void downloadMetaData(String dateToDownload) throws FireRiskException
    {
        try
        {
            // date format for geoMeta file, if needed
            if (dateToDownload == null || dateToDownload.equals(""))
            {
                DateFormat dateFormat = new SimpleDateFormat(GEO_META_FILENAME_DATE_FORMAT);
                dateToDownload = dateFormat.format(new Date());

                // set currentYear to date sent in (just in case you are getting data for last year)
                currentYear = dateToDownload.substring(0, 4);
            }

            // set up temporary geoMeta file that will be downloaded
            File tempMetaDataFile = new File(TEMP_FILENAME);

            // generate directory and filename for geoMeta file
            String directory = META_DATA_DIRECTORY + currentYear;
            String filename = GEO_META_FILENAME_PREFIX + dateToDownload + GEO_META_FILENAME_SUFFIX;

            // download the metadata file
            tempMetaDataFile = FtpHelper.getFTPFile(MOD7_FTP_SITE, directory, "anonymous", "", filename, tempMetaDataFile);

            metaDataDownloaded = true;

            // create the buffered reader to read the metadata from
            metaDataReader = new BufferedReader(new FileReader(tempMetaDataFile));
        }
        catch (FTPException ftpe)
        {
            System.out.println("FTPExeption thrown attempting to initialize fire data.");
            throw new FireRiskException(ftpe);
        }
        catch (FileNotFoundException fnfe)
        {
            System.out.println("FileNotFoundException thrown attempting to create the file reader from the temp fire.");
            throw new FireRiskException(fnfe);
        }
    }

    public void downloadMOD7Data(String downloadFile) throws FireRiskException
    {
        if (!metaDataDownloaded)
        {
            throw new IllegalStateException("You must download the meta data before you can download the MOD7 data.");
        }

        try
        {
            OutputStream output = new FileOutputStream(downloadFile);

            String next;
            String[] metaData;
            float gRingLongitude1;
            float gRingLongitude2;
            float gRingLongitude3;
            float gRingLongitude4;
            float gRingLatitude1;
            float gRingLatitude2;
            float gRingLatitude3;
            float gRingLatitude4;
            String[] mod7DataFileParts;

//            String mod7DataFileDirectory = MOD7_DATA_DIRECTORY + "/" + currentYear + "/" + generateDayOfYear() + "/";

            while((next = metaDataReader.readLine()) != null)
            {
                if (!next.startsWith(COMMENT_PREFIX))
                {
                    metaData = next.split(META_DATA_DELIMITER);
                    if (metaData[DAY_NIGHT_INDEX].trim().equalsIgnoreCase(DAY_VALUE))
                    {
                        gRingLongitude1 = Float.valueOf(metaData[GRING_LONG_1_INDEX]).floatValue();
                        gRingLongitude2 = Float.valueOf(metaData[GRING_LONG_2_INDEX]).floatValue();
                        gRingLongitude3 = Float.valueOf(metaData[GRING_LONG_3_INDEX]).floatValue();
                        gRingLongitude4 = Float.valueOf(metaData[GRING_LONG_4_INDEX]).floatValue();
                        gRingLatitude1 = Float.valueOf(metaData[GRING_LAT_1_INDEX]).floatValue();
                        gRingLatitude2 = Float.valueOf(metaData[GRING_LAT_2_INDEX]).floatValue();
                        gRingLatitude3 = Float.valueOf(metaData[GRING_LAT_3_INDEX]).floatValue();
                        gRingLatitude4 = Float.valueOf(metaData[GRING_LAT_4_INDEX]).floatValue();

                        if ((
                                (gRingLongitude1 >= -180.0 && gRingLongitude1 <= 180.0) ||
                                (gRingLongitude2 >= -180.0 && gRingLongitude2 <= 180.0) ||
                                (gRingLongitude3 >= -180.0 && gRingLongitude3 <= 180.0) ||
                                (gRingLongitude4 >= -180.0 && gRingLongitude4 <= 180.0)
                           ) &&
                           (
                                (gRingLatitude1 >= -28.162 && gRingLatitude1 <= 13.066) ||
                                (gRingLatitude2 >= -28.162 && gRingLatitude2 <= 13.066) ||
                                (gRingLatitude3 >= -28.162 && gRingLatitude3 <= 13.066) ||
                                (gRingLatitude4 >= -28.162 && gRingLatitude4 <= 13.066)
                           ))
                        {
                            mod7DataFileParts = metaData[FILENAME_INDEX].split(DATA_FILE_DELIMITER);
                            StringBuffer mod7DataFile = new StringBuffer();
                            mod7DataFile.append(DATA_FILE_PREFIX);
                            mod7DataFile.append(FILENAME_SEPARATOR);
                            mod7DataFile.append(mod7DataFileParts[1]);
                            mod7DataFile.append(FILENAME_SEPARATOR);
                            mod7DataFile.append(mod7DataFileParts[2]);
                            mod7DataFile.append(FILENAME_SEPARATOR);
                            mod7DataFile.append(mod7DataFileParts[3]);
                            mod7DataFile.append("*");
                            mod7DataFile.append(NEW_LINE);

                            output.write(mod7DataFile.toString().getBytes());
                        }
                    }
                }
            }

            output.flush();
            output.close();

            metaDataReader.close();
        }
        catch (IOException ioe)
        {
           throw new FireRiskException(ioe);
        }
        catch (Exception e)
        {
           throw new FireRiskException(e);
        }
    }

//    protected String generateDayOfYear()
//    {
//        Calendar calendar = new GregorianCalendar();
//        String dayOfYear = String.valueOf(calendar.get(Calendar.DAY_OF_YEAR));
//        dayOfYear = dayOfYear.replaceAll(TWO_DIGITS_REPLACEMENT_REG_EX, TWO_DIGITS_PREFIX + dayOfYear);
//        dayOfYear = dayOfYear.replaceAll(ONE_DIGIT_REPLACEMENT_REG_EX, ONE_DIGIT_PREFIX + dayOfYear);
//
//        return dayOfYear;
//    }

}
