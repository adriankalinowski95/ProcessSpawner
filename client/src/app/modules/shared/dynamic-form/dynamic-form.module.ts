import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { DynamicFormComponent } from './dynamic-form.component';
import {FormsModule, ReactiveFormsModule} from "@angular/forms";
import { MatFormFieldModule} from "@angular/material/form-field";
import {DynamicFormContentComponent} from "./dynamic-form-content/dynamic-form-content.component";
import {DynamicFormActionPanelComponent} from "./dynamic-form-action-panel/dynamic-form-action-panel.component";
import {ActionButtonModule} from "../action-button/action-button.module";
import {ValidationMessageComponent} from "../validation-message/validation-message.component";
import {
  DynamicFormValidationMessageComponent
} from "./dynamic-form-validation-message/dynamic-form-validation-message.component";



@NgModule({
  declarations: [
    DynamicFormComponent,
    DynamicFormContentComponent,
    DynamicFormActionPanelComponent,
    DynamicFormValidationMessageComponent
  ],
  exports: [
    DynamicFormComponent
  ],
  imports: [
    CommonModule,
    ReactiveFormsModule,
    FormsModule,
    MatFormFieldModule,
    ActionButtonModule,
    ValidationMessageComponent
  ]
})
export class DynamicFormModule { }
