public class ComplementReverse {
    public static void main(String[] args){
        System.out.println(complementReverse(37));
        return;
    }
    //10진수를 2진수로 바꾼 후, 1의 보수 -> 뒤집기 한것을 다시 10진수로
    public static int complementReverse(int num){
        String arr = Integer.toBinaryString(num);
        System.out.println(arr);
        StringBuilder one=new StringBuilder();
        for(int i=0;i<arr.length();i++) {
            if (arr.charAt(i) == '0') one.append(1);
            else one.append(0);
        }
        one.reverse();
        return Integer.parseInt(one.toString(),2);
    }
}
