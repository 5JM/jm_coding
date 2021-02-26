public class LongestPalindromicSubstr {
    public  static void main(String[] args){
        System.out.println(longestPalindrome("a"));
//        System.out.println(checkPal(""));
        return;
    }
    public static String longestPalindrome(String s) {
        if(s.length()<2) return s;
        String result="";
        for(int k=2;k<=s.length();k++) {
            for (int i = 0; i <= s.length()-k; i++) {
                if(checkPalindrome(s.substring(i, i + k))) {
                    //System.out.println(s.substring(i, i + k));
                    if(result.length()<s.substring(i, i + k).length())
                        result=s.substring(i, i + k);
                }
            }
        }
        return result==""?""+s.charAt(0):result;
    }
    static boolean checkPalindrome(String s){
        for(int i=0;i<s.length()/2;i++) {
            if (s.charAt(i) != s.charAt(s.length() - 1 - i)) return false;
            //파라미터로 들어온 문자열 s의 양끝을 검사한 후, 양끝을 잘라 다시 checkPalindrome 함수에
            checkPalindrome(s.substring(i + 1, s.length() - i - 1));
        }
        return true;
    }
}
