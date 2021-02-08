public class PalindromeSubstring {
    //주어진 문자열 속 회문 찾기
    //조건 : 문자속 회문의 길이는 최소 3
    public static void main(String[] args){
        System.out.println(findPalindrome("abbaa"));
        System.out.println(findPalindrome("IAEFGSADAOFSOSPIPOKTOOTSCIVIC"));
        return;
    }
    public static int findPalindrome(String s){
        int count=0;
        for(int k=3;k<=s.length();k++) {
            for (int i = 0; i <= s.length()-k; i++) {
                if(checkPalindrome(s.substring(i, i + k))) {
                    System.out.println(s.substring(i, i + k));
                    count++;
                }
            }
        }
        return count;
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
