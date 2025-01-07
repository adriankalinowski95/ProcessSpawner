import {Component, Input} from '@angular/core';
import {FormControl, FormsModule, ReactiveFormsModule} from "@angular/forms";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatRadioModule} from "@angular/material/radio";
import {CustomDropDownData} from "../custom-dropdown/custom-dropdown.component";
import {MatOption} from "@angular/material/autocomplete";
import {MatSelect} from "@angular/material/select";

@Component({
  selector: 'app-custom-radio-button',
  standalone: true,
  imports: [
    ReactiveFormsModule,
    FormsModule,
    MatRadioModule,
    MatFormFieldModule,
    MatOption,
    MatSelect
  ],
  templateUrl: './custom-radio-button.component.html',
  styleUrl: './custom-radio-button.component.scss'
})
export class CustomRadioButtonComponent {
  @Input() data: CustomDropDownData;
  @Input() control: FormControl;

  ngOnInit() {
    this.control.markAsTouched()
  }
}

export interface CustomRadioButtonData {
  key: string;
  label: string;
  tooltip?: string;
  value: string|number;
  options: {
    id: string | number,
    value: string;
    tooltip?: string;
  }[]
}
