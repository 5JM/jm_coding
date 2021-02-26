public class ImplementStr {
    public static void main(String[] args){
        System.out.println(strStr("",""));
        return;
    }
    //haystack에서 needle이 포함된 스트링의 시작 index반환, 없으면 -1, needle이 ""면 0반환
    public static int strStr(String haystack, String needle) {
        if (needle=="")
            return 0;
        if(haystack.contains(needle)){
            return haystack.indexOf(needle);
        }
        return -1;
    }
}
