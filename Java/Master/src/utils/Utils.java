package utils;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class Utils {
	private static final String ERROR = "ERROR";
	private static final String INFO = "INFO";
	private static final String WARNING = "WARNING";
	
	public static void logError(String msg){
		System.out.println(ERROR + ":" + msg);
	}
	
	public static void logInfo(String msg){
		System.out.println(INFO + ":" + msg);
	}
	
	public static void logWarning(String msg){
		System.out.println(WARNING + ":" + msg);
	}
	
	public static List<String> readFileContent(String fileName){
		List<String> fileContents = null;
		BufferedReader bufferedReader = null;
		try {
			FileInputStream fileInputStream = new FileInputStream(fileName);
			InputStreamReader inputStream = new InputStreamReader(fileInputStream);
			bufferedReader = new BufferedReader(inputStream);
			fileContents = new ArrayList<>();
			String line;
			while ((line = bufferedReader.readLine()) != null) {
				fileContents.add(line);
			}
		} catch (FileNotFoundException e) {
			Utils.logError("readFileContent(String fileName) FileNotFound");
			e.printStackTrace();
		} catch (IOException e) {
			Utils.logError("readFileContent(String fileName) IOException");
			e.printStackTrace();
		} finally {
			try {
				bufferedReader.close();
			} catch (IOException e) {
				Utils.logError("readFileContent(String fileName) when closing bufferedReader");
				e.printStackTrace();
			}
		}
		
		return fileContents;
	}
	
	public static boolean writeFile(List<String> fileData, String fileName){
		BufferedWriter bufferedWriter = null;
		FileWriter fileWriter = null;
		try {
			fileWriter = new FileWriter(fileName);
			bufferedWriter = new BufferedWriter(fileWriter);
			
			bufferedWriter.write(fileData.size());
			
			for(String string : fileData){
				bufferedWriter.write(string);
			}
			
			return true;
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if (bufferedWriter != null)
					bufferedWriter.close();
				if (fileWriter != null)
					fileWriter.close();
			} catch (IOException ex) {
				Utils.logError("writeFile(List<String> fileData, String fileName) when writing");
				ex.printStackTrace();
			}
		}
		return false;
	}
}
