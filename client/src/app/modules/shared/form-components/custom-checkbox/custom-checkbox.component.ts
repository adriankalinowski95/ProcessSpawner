import {ChangeDetectionStrategy, Component, Input } from '@angular/core';
import {
  FormControl,
  FormsModule,
  ReactiveFormsModule
} from '@angular/forms';
import {MatCheckboxModule} from '@angular/material/checkbox';
import { MatFormFieldModule} from "@angular/material/form-field";

@Component({
  selector: 'app-custom-checkbox',
  templateUrl: './custom-checkbox.component.html',
  standalone: true,
  imports: [FormsModule, MatCheckboxModule, MatFormFieldModule, ReactiveFormsModule],
  styleUrl: './custom-checkbox.component.scss',
  changeDetection: ChangeDetectionStrategy.OnPush,
})
export class CustomCheckboxComponent {
  @Input() data: CustomCheckboxData;
  @Input() control: FormControl;
}
export interface CustomCheckboxData {
  tooltip?: string;
  key: string;
  label: string;
  value: boolean
}
