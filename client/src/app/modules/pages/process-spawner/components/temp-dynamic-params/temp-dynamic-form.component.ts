import { Component } from '@angular/core';
import { DialogComponentRef } from '../../../../shared/dialog/dialog-component-ref';
import { FormArray, FormBuilder, FormGroup, Validators } from '@angular/forms';

enum BaseFieldUIType {
    String = "String",
    DropDown = "DropDown",
    Boolean = "Boolean"
};

@Component({
  selector: 'app-temp-dynamic-form',
  templateUrl: './temp-dynamic-form.component.html',
  styleUrl: './temp-dynamic-form.component.scss'
})
export class TempDynamicFormComponent {
    BaseFieldUIType = BaseFieldUIType;
    dynamicForm: FormGroup;
    
    constructor(private formBuilder: FormBuilder) {
      // super();
      this.dynamicForm = this.formBuilder.group({
        items: this.formBuilder.array([]),
      });
    }

    get items(): FormArray {
      return this.dynamicForm.get('items') as FormArray;
    }

    addItem(): void {
      const itemGroup = this.formBuilder.group({
        key: ['', Validators.required],
        type: [BaseFieldUIType.String, Validators.required],
        defaultValue: [''],
        options: this.formBuilder.array([]), // Dla typu DropDown
      });   
      this.items.push(itemGroup);
    }

    removeItem(index: number): void {
      this.items.removeAt(index);
    }

    getOptions(itemIndex: number): FormArray {
      return this.items.at(itemIndex).get('options') as FormArray;
    }

    addOption(itemIndex: number): void {
      const optionGroup = this.formBuilder.group({
        key: ['', Validators.required],
        value: ['', Validators.required],
      });
      this.getOptions(itemIndex).push(optionGroup);
    }

    removeOption(itemIndex: number, optionIndex: number): void {
      this.getOptions(itemIndex).removeAt(optionIndex);
    }

    onTypeChange(itemIndex: number): void {
      const item = this.items.at(itemIndex);
      const type = item.get('type')?.value; 

      item.get('defaultValue')?.setValue('');
      const options = item.get('options') as FormArray;
      options.clear();  
      if (type === BaseFieldUIType.DropDown) {
        this.addOption(itemIndex);
      }
    }
    
    submit(): void {
      console.log(this.dynamicForm.value);
    }
}