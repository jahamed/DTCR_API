import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import com.jinvoke.CallingConvention;
import com.jinvoke.JInvoke;
import com.jinvoke.NativeImport;
import com.jinvoke.Charset;

public class DTCR_API_Java {
	@NativeImport(library="./DTCR_API.dll", function="DefineElement", charset=Charset.ANSI)
		public static native int DefineElement(String equipID, String EDS, int maxMsg, StringBuffer errorMsg);

	@NativeImport(library="./DTCR_API.dll", function="ComputeTransformerRating2", charset=Charset.ANSI)
		public static native int ComputeTransformerRating2(String equipID, float duration, 
				float onset, String ratingShape, String loadDefString, String ambientDefString,
				float maxHS, float maxTO, float maxLL, int maxMsg, int maxLFL, float[] ratingMVA,
				float[] ratingAMPS, float[] maxIncHS, float[] maxIncTO, float[] maxIncLL, float[] maxIncBO,
				StringBuffer errorMsg, StringBuffer limitingFactor, float[] maxH, float[] maxX, float[] maxY);

	private static String readFileAsString(String filePath) throws java.io.IOException{
		byte[] buffer = new byte[(int) new File(filePath).length()];
		BufferedInputStream f = new BufferedInputStream(new FileInputStream(filePath));
		f.read(buffer);
		return new String(buffer);
	}

	public static void main(String[] args){
		BufferedReader inputer = new BufferedReader(new InputStreamReader(System.in));
		String equipID = null;
		Float duration = null, onset = null, airTemp = null, maxHS = null, maxTO = null, maxLL = null;
		int maxMsgLength = 2000;
		int maxLimFactorLength = 40;
		int returnCode;

		//These are float* in C++, in Java they must be declared as arrays with single elements
		float[] ratingMVA = {0};
		float[] ratingAMPS = {0};
		float[] maxIncHS = {0};
		float[] maxIncTO = {0}; 
		float[] maxIncLL = {0};
		float[] maxIncBO = {0};
		float[] maxH = {0};
		float[] maxX = {0};
		float[] maxY = {0};

		//These are char*, we have to use StringBuffer since strings are immutable
		StringBuffer limitingFactor = new StringBuffer(2000);
		StringBuffer errorMsg = new StringBuffer(2000);

		//Get EDS/LDS/ADS from respective files
		String EDS = null, LDS = null, ADS = null;
		try {
			EDS = readFileAsString("EDS.txt");
		} catch (IOException e) {
			System.out.println("FILE INPUT CRASH (EDS)");
			e.printStackTrace();
		}
		try {
			LDS = readFileAsString("LDS.txt");
		} catch (IOException e) {
			System.out.println("FILE INPUT CRASH (LDS)");
			e.printStackTrace();
		}
		try {
			ADS = readFileAsString("ADS.txt");
		} catch (IOException e) {
			System.out.println("FILE INPUT CRASH (ADS)");
			e.printStackTrace();
		}

		JInvoke.initialize();

		//Start asking user for input
		System.out.println("Element Definition Strings: "); //Got From File
		System.out.println("Load Definition String: "); //Got From File
		System.out.println("Ambient Definition String: "); //Got From File
		System.out.println("EquipID: ");
		try {
			equipID = inputer.readLine();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Duration (hours): ");
		try {
			duration = Float.valueOf(inputer.readLine());
		} catch (NumberFormatException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Onset (hours): ");
		try {
			onset = Float.valueOf(inputer.readLine());
		} catch (NumberFormatException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Air Temp (c): ");
		try {
			airTemp = Float.valueOf(inputer.readLine());
		} catch (NumberFormatException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Max HS (c): ");
		try {
			maxHS = Float.valueOf(inputer.readLine());
		} catch (NumberFormatException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Max TO (c): ");
		try {
			maxTO = Float.valueOf(inputer.readLine());
		} catch (NumberFormatException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Max LL (%): ");
		try {
			maxLL = Float.valueOf(inputer.readLine());
		} catch (NumberFormatException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		returnCode = DefineElement(equipID, EDS, maxMsgLength, errorMsg);
		System.out.println("The DefineElement returnCode was: "+returnCode);
		System.out.println("The DefineElement errorMsg was: "+errorMsg.toString());

		returnCode = ComputeTransformerRating2(equipID, duration, onset, "FLAT_SPIKE", LDS, ADS, maxHS, maxTO, maxLL, maxMsgLength, maxLimFactorLength, 
				ratingMVA, ratingAMPS, maxIncHS, maxIncTO, maxIncLL, maxIncBO, errorMsg, limitingFactor,
				maxH, maxX, maxY);
		System.out.println("The ComputeTransformerRating2 returnCode was: "+returnCode);
		System.out.println("The DefineElement errorMsg was: "+errorMsg.toString());

		System.out.println("ratingMVA = " + ratingMVA[0]);
		System.out.println("ratingAMPS = " + ratingAMPS[0]);
		System.out.println("maxIncHS = " + maxIncHS[0]);
		System.out.println("maxIncTO = " + maxIncTO[0]);
		System.out.println("maxIncLL = " + maxIncLL[0]);
		System.out.println("maxIncBO = " + maxIncBO[0]);
		System.out.println("maxH = " + maxH[0]);
		System.out.println("maxX = " + maxX[0]);
		System.out.println("maxY = " + maxY[0]);
	}

}
