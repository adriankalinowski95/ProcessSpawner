import {ComponentRef} from "@angular/core";
import {AsyncValidatorFn, ValidatorFn} from "@angular/forms";
import {Config} from "../../action-button/models/action-button-config";

export interface DynamicFormConfig {
  stepper: boolean;
  stepperConfig?: StepperConfig;
  data: FormGroupConfig[];
  errorHandlerType: ErrorHandlerType;
  title: string;
  actionPanelConfig: ActionPanelConfig
}

export interface ActionPanelConfig {
  actionPanel?: Config[];
  actionHandlerType: ActionHandlerType
}
export interface NestedDynamicFormConfig {
  stepper: boolean;
  stepperConfig?: StepperConfig;
  data: FormGroupConfig[];
  title: string;
}

export enum ErrorHandlerType {
  None,
  Inner,
  Outer
}
export enum ActionHandlerType {
  Inner,
  Outer,
  Default
}


export interface FormControlData {
  key: string;
  value: FormDataValueType;
  tooltip?: string;
  label: string;
  guiValidators: GuiValidator[];
  customValidators: CustomValidator[];
  type: DynamicFormControlType,
  component?: FormControlComponent;
  dataSource?: any[];
}

export class FormControlComponent {
  static data: any
}

export interface FormGroupConfig {
  groupName: string;
  groupTitle?: string;
  formControlsData: FormControlData[];
  nestedForm?: NestedDynamicFormConfig;
}

// export interface FormData {
//   key: string;
//   value: FormDataValueType;
//   tooltip?: string;
//   label: string;
//   guiValidators: GuiValidator[];
//   customValidators: CustomValidator[];
//   hasChildForm: boolean;
//   type: DynamicFormControlType,
//   data?: DynamicFormConfig;
//   componentRef?: ComponentRef<any>;
// }


type FormDataValueType = string | [] | boolean | number

export interface StepperConfig {

}

export interface CustomValidator {
  type: CustomValidatorType
  validationFunction: ValidatorFn | AsyncValidatorFn;

}

export enum CustomValidatorType {
  Sync,
  Async
}
export enum DynamicFormControlType {
  Checkbox,
  Dropdown,
  Input,
  Radiobutton,
  Custom,
}

export interface GuiValidator {
  value: string | number | boolean;
  type: GuiValidatorType;
}
export enum GuiValidatorType {
  MaxLength,
  MinLength,
  Required,
  Email
}
