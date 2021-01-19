import java.util.Arrays;
//문자열 압축 - 카카오 기출
public class test {
    public static void main(String args[]){
        System.out.println(sol("ababcdcdababcdcd"));
        return;
    }
    static int sol(String str){
        //길이가 1일때 바로 리턴.
        if (str.length() == 1) return 1;
        int count=0;
        int len=str.length();
        int arrLen;
        int result=0;
        int [] findMin= new int[len/2];
        for(int i=0;i<len/2;i++){
            //문자의 길이
            count++;
            //글자수의 길이만큼 배열의 크기 설정
            if(len%count!=0) arrLen=len/count+1;
            else arrLen=len/count;

            //문자열 자르기
            int index=0;
            String [] s=new String[arrLen];
            for(int j=0;j<s.length;j++){
                //인덱스 익셉션 방지
                if(index+count>len) s[j]=str.substring(index,len);
                else s[j]=str.substring(index,index+count);

                index+=count;
            }
            int counting =1;
            StringBuilder newStr = new StringBuilder();
            for(int j=0; j<s.length-1;j++){
                if(s[j].equals(s[j+1])){
                    counting++;
                }else{
                    if(counting!=1) newStr.append(counting).append(s[j]);
                    else newStr.append(s[j]);
                    counting=1;
                }
            }
            if(counting!=1) newStr.append(counting).append(s[s.length - 1]);
            else newStr.append(s[s.length - 1]);
            System.out.println(newStr);
            findMin[i] = newStr.length();
        }
        Arrays.sort(findMin);
        result=findMin[0];
        return result;
    }
}
