public class LongestCommonProfix {
    public static void main(String[] args){
        String[] str = {"flower","flow","flight"};
        System.out.println(longestCommonPrefix(str));
        return ;
    }
    public static String longestCommonPrefix(String[] strs){
        if(strs.length == 0) return "";
        String min = "";

        for(int i = 0; i < strs.length-1; i++){
            if(strs[i].length() < strs[i+1].length()){
                min = strs[i];
            }else{
                min = strs[i+1];
            }
        }

        for(int i = 0; i < strs.length; i++){
            if(!strs[i].startsWith(min)){
                i = -1;
                min = min.substring(0,min.length()-1);
            }
        }
        return min;
    }
}
