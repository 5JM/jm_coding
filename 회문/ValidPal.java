public class ValidPal {
    public static void main(String[] args){
        System.out.println(isPalindrome2("..abc.."));
        return;
    }
    public static boolean isPalindrome(String s) {
        if(s.equals(""))return false;
        if(s.equals(" ")||s.length()<2)return true;
        s=s.replaceAll(" ","");
        s=s.toLowerCase();
        StringBuilder sb=new StringBuilder(s);
        for(int i=0;i<sb.length();i++) {
            if (!((sb.charAt(i)>=65&&sb.charAt(i)<=90)||(sb.charAt(i)>=97&&sb.charAt(i)<=122)||(sb.charAt(i)>=48&&sb.charAt(i)<=57))){
                sb.deleteCharAt(i);
                i--;
            }
        }
        if(sb.length()==1) {
            System.out.println(sb.toString());
            return true;
        }
        String temp = sb.toString();
        if(!temp.equals(sb.reverse().toString())) return false;
        return true;
    }
    //best sol
    public static boolean isPalindrome2(String s) {
        int left = 0, right = s.length()-1;
        while(left < right) {
            if (!Character.isLetterOrDigit(s.charAt(left))) {
                left++;
                continue;
            }
            if (!Character.isLetterOrDigit(s.charAt(right))) {
                right--;
                continue;
            }
            if (Character.toLowerCase(s.charAt(left)) != Character.toLowerCase(s.charAt(right))) {
                return false;
            }
            left++;
            right--;
        }
        return true;

    }
}
