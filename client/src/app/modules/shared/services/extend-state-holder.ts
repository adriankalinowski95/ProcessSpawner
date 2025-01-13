import { BaseStateHolder } from "./base-state-holder";

export class ExtendStateHolder<T extends { id: any }> extends BaseStateHolder<T> {
    selectById(id: any) : boolean {
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
    
    deleteById(id: any) : boolean {
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

    updateById(item: T) : boolean {
        const items = this.getLastItems();
        if (items.length == 0) {
          return false;
        }
    
        const itemIndex = items.findIndex(i => i.id == item.id);
        if (itemIndex == -1) {
          return false;
        }
    
        items[itemIndex] = item;
        this.setItems(items);
    
        return true;
    }

    getById(id: any) {
        return this.itemsSource.value.find(item => item.id == id);
    }
}