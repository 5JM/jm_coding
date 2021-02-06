public class RomanToInt {
    public static void main(String[] args){
        System.out.println(romanToInt("III"));
        return ;
    }
    //I = 1
    //V = 5
    //X = 10
    //L = 50
    //C = 100
    //D = 500
    //M = 1000  1<=s.length<=15, s' range : [1~3999] = I ~ IIVM
    public static int romanToInt(String s){
        int result = 0;
        for(int i=0;i<s.length();i++){
            char current=s.charAt(i);
            switch (current){
                case 'I':
                    result+=1;
                    break;
                case 'V':
                    if(i>0 && s.charAt(i-1) == 'I')
                        result+=3;
                    else
                        result+=5;
                    break;
                case 'X':
                    if(i>0 && s.charAt(i-1) == 'I')
                        result+=8;
                    else
                        result+=10;
                    break;
                case 'L':
                    if(i>0 && s.charAt(i-1) == 'X')
                        result+=30;
                    else
                        result+=50;
                    break;
                case 'C':
                    if(i>0 && s.charAt(i-1) == 'X')
                        result+=80;
                    else
                        result+=100;
                    break;
                case 'D':
                    if(i>0 && s.charAt(i-1) == 'C')
                        result+=300;
                    else
                        result+=500;
                    break;
                case 'M':
                    if(i>0 && s.charAt(i-1) == 'C')
                        result+=800;
                    else
                        result+=1000;
                    break;
            }
        }
        return result;
    }
}
