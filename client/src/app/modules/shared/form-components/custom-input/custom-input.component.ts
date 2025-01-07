import {ChangeDetectionStrategy, Component, forwardRef, Input} from '@angular/core';
import {MatInputModule} from '@angular/material/input';
import {ControlValueAccessor, FormControl, FormsModule, NG_VALUE_ACCESSOR} from '@angular/forms';
import {MatFormFieldModule} from "@angular/material/form-field";
import {ReactiveFormsModule} from '@angular/forms';
@Component({
  selector: 'app-custom-input',
  standalone: true,
  imports: [MatInputModule, FormsModule, MatFormFieldModule, ReactiveFormsModule],
  templateUrl: './custom-input.component.html',
  styleUrl: './custom-input.component.scss',
})
export class CustomInputComponent  {
  @Input() data: CustomInputData;
  @Input() control: FormControl;

  ngOnInit() {
    this.control.markAsTouched()
  }
}

export interface CustomInputData {
  tooltip?: string;
  key: string;
  label: string;
  value: string;
}
