public class FindLongestSubstring {
    public static void main(String args[]){
        System.out.println(lengthOfLongestSubstring("abcabcbb"));
        return;
    }
    //abcabcqwerty
    static public int lengthOfLongestSubstring(String s) {
        char[] chars = s.toCharArray();
        int start = 0;
        int max = 0;
        for (int i = 0; i < s.length(); i++) {
            for (int j = start; j < i; j++) {
                if (chars[i] == chars[j]) {
                    if (i - start > max) { max = i - start; }
                    start = j + 1;
                    break;
                }
            }
        }
        //문자열 뒷부분이 반복되는 부분이 없을때 ex. abcabcqwerty
        if(s.length() - start > max) { max = s.length() - start; }
        return max;
    }
}
