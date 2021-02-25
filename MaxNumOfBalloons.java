import java.util.HashMap;

public class MaxNumOfBalloons {
    public static void main(String[] args){
        System.out.println(maxNumberOfBalloons2("balloonsadf"));
        return;
    }

    public static int maxNumberOfBalloons2(String text) {
        int result = 0;
        char count[]=new char[26];
        for(int i=0;i<text.length();i++) count[text.charAt(i)-'a']++;

        String res="balloon";
        while(true){
            for(int i=0;i<res.length();i++) if (count[res.charAt(i) - 'a']-- <= 0) return result;
            result++;
        }
    }
}
