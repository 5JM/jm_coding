public class SearchInsertPosition {
    public static void main(String[] args){
        int[] arr={1,3,5,6};
        //int[] arr={1};
        System.out.println(searchInsert(arr,4));
        return;
    }
    //Given a sorted array of distinct integers and a target value,
    //return the index if the target is found.
    //If not, return the index where it would be if it were inserted in order.
    public static int searchInsert(int[] nums, int target) {
        if(target < nums[0])
            return 0;
        else if(target > nums[nums.length - 1])
            return nums.length;

        int result=0;
        for(int i=0;i< nums.length;i++){
            if(nums[i]==target) {
                result = i;
                break;
            }
            if(nums[i] < target && nums[i + 1] > target && i != nums.length - 1){
                result  = i + 1;
                break;
            }
        }
        return result;
    }
}
