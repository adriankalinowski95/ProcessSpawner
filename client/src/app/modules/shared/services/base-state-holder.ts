import { BehaviorSubject } from "rxjs";

export class BaseStateHolder<T> {
  protected itemsSource = new BehaviorSubject<Array<T>>([]);
  items$ = this.itemsSource.asObservable();

  protected selectedItemSource = new BehaviorSubject<T | null>(null);
  selectedItem$ = this.selectedItemSource.asObservable();

  constructor() { }

  public setItems(items: Array<T>) {
    this.itemsSource.next(items);
  }

  public getLastItems() {
    return this.itemsSource.value;
  }

  public getLastSelectedItem() {
    return this.selectedItemSource.value;
  }

  public selectItem(item: T | null) {
    this.selectedItemSource.next(item);
  }

  public updateSelectedItem(item:T){
    this.selectedItemSource.next(item);
  }

  public addItem(item:T) {
    this.itemsSource.next([...this.itemsSource.getValue(), item]);
  }

  public clear() {
    this.itemsSource.next([]);
    this.selectedItemSource.next(null);
  }
}
