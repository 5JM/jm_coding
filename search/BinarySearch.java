public class BinarySearch {
    public static void main(String[] args){
        return;
    }
    private static int search(int[] nums, int target) {
        int left=0;
        int right= nums.length-1;
        int pivot ;
        while(left<=right){
            pivot=(left+right)/2;
            if(target==nums[pivot]){
                return pivot;
            }else if(target<nums[pivot])
                right=pivot-1;
            else left=pivot+1;
        }
        return -1;
    }
}
