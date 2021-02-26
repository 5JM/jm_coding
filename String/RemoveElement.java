public class RemoveElement {
    public static void main(String[] args){
        int[] arr ={1};
//        int[] arr ={3,2,2,3};
        System.out.println(removeElement2(arr,1));
        return;
    }

    public static int removeElement(int[] nums, int val) {
        int i = 0;
        //RemoveDup과 방식이 비슷
        //불필요한 복사가 일어남 ex) nums={4,1,2,3,5}, val = 4
        for (int j = 0; j < nums.length; j++) {
            if (nums[j] != val) {
                nums[i] = nums[j];
                i++;
            }
        }
        return i;
    }
    public static int removeElement2(int[] nums, int val) {
        //불필요한 복사 X
        int i = 0;
        int n = nums.length;
        while (i < n) {
            if (nums[i] == val) {
                nums[i] = nums[n - 1];
                n--;
            } else {
                i++;
            }
        }
        return n;
    }
}
