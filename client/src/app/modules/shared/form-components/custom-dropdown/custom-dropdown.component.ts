import {Component, Input} from '@angular/core';
import {FormControl, FormsModule, ReactiveFormsModule} from '@angular/forms';
import {MatSelectModule} from '@angular/material/select';
import {MatFormFieldModule} from '@angular/material/form-field';
@Component({
  selector: 'app-custom-dropdown',
  standalone: true,
    imports: [
      ReactiveFormsModule,
      FormsModule,
      MatSelectModule,
      MatFormFieldModule
    ],
  templateUrl: './custom-dropdown.component.html',
  styleUrl: './custom-dropdown.component.scss'
})
export class CustomDropdownComponent {
  @Input() data: CustomDropDownData;
  @Input() control: FormControl;

  ngOnInit() {
    this.control.markAsTouched()
  }
}
export interface CustomDropDownData {
  key: string;
  label: string;
  tooltip?: string;
  value: string|number;
  options: {
    id: string | number,
    value: string;
  }[]
}
