public class AddNumIsPal {
    //한개의 정수를 뒤집은 수와 더해 나온 수가 회문인지 판별
    //Ex) 120 + 021 = 141 -> true
    public static void main(String[] args){
        System.out.println(addNumIsPal(121));
        System.out.println(addNumIsPal(614));
        System.out.println(addNumIsPal(120));
        System.out.println(addNumIsPal(46));
        System.out.println(addNumIsPal(17));
        return;
    }
    public static boolean addNumIsPal(int a){
        String str1 = Integer.toString(a);
        StringBuilder str2 =new StringBuilder();
        for(int i=str1.length()-1;i>=0;i--)
            str2.append(str1.charAt(i));
        String result = Integer.toString(a+Integer.parseInt(str2.toString()));
        System.out.println(result);
        for(int i=0;i<result.length()/2;i++){
            if(result.charAt(i)!=result.charAt(result.length()-1)) return false;
        }
       return true;
    }
}
