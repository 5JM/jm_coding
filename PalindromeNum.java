public class PalindromeNum {
    public static void main(String[] args) {
        System.out.println(isPalindrome(-101));
        return;
    }
    //정수에서 순회이면 true
    //만약 입력값이 -101일때 왼쪽부터 읽으면 -101, 오른쪽부터 읽으면 101- 이라서 false
    public static boolean isPalindrome(int x) {
        if(x<0) return false;
        long revX=0;
        int tmp=x;
        while(x!=0){
           revX*=10;
           revX+=x%10;
            x=x/10;
        }
        if(revX<Integer.MIN_VALUE||revX>Integer.MAX_VALUE){
            return false;
        }
        return tmp == revX;
    }
}