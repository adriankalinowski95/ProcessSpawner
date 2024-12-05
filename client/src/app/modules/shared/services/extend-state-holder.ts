import { BaseStateHolder } from "./base-state-holder";

export class ExtendStateHolder<T extends { id: any }> extends BaseStateHolder<T> {
    selectById(id: number) : boolean {
        const items = this.getLastItems();
        if (items.length == 0) {
          return false;
        }
    
        const item = items.find(item => item.id == id);
        if (!item) {
          return false;
        }
    
        this.selectItem(item);
    
        return true;
    }
    
    deleteById(id: number) : boolean {
        const items = this.getLastItems();
        if (items.length == 0) {
          return false;
        }
    
        const itemIndex = items.findIndex(item => item.id == id);
        if (itemIndex == -1) {
          return false;
        }
    
        items.splice(itemIndex, 1);
        this.setItems(items);
    
        return true;
    }
}