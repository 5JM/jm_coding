public class ReverseInt {
    public static void main(String[] args){
        System.out.println(reverse(1534236469));
        return ;
    }
    
    public static int reverse(int x) {
        long result = 0;
        while(x!=0){
            result*=10;
            result += x % 10;
            x=x/10;
        }
        if(result<Integer.MIN_VALUE||result>Integer.MAX_VALUE)
            return 0;
        else
            return (int)result;
    }
}
