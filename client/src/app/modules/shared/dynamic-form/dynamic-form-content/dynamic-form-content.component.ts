import {Component, Input} from '@angular/core';
import {NgComponentOutlet, NgTemplateOutlet} from "@angular/common";
import {AsyncValidatorFn, FormControl, FormGroup, ReactiveFormsModule, ValidatorFn, Validators} from "@angular/forms";
import {
  CustomValidatorType,
  DynamicFormConfig, DynamicFormControlType,
  FormControlData,
  GuiValidator,
  GuiValidatorType,
  NestedDynamicFormConfig
} from "../models/dynamic-form-config";
import {tap} from "rxjs";

@Component({
  selector: 'app-dynamic-form-content',
  templateUrl: './dynamic-form-content.component.html',
  styleUrl: './dynamic-form-content.component.scss'
})
export class DynamicFormContentComponent {
  @Input() data: DynamicFormConfig

  form: FormGroup = new FormGroup({});
  templateSchema: TemplateSchema[] = [];
  templateGroupType = TemplateGroupType;

  ngOnInit() {
    this.dynamicFormBuilder(this.form, this.templateSchema, this.data);
    this.form.valueChanges.pipe(tap(v => console.log('changes', v))).subscribe()
  }


  dynamicFormBuilder(form: FormGroup, template: any[], formConfig: NestedDynamicFormConfig) {
    formConfig.data.forEach(groupConfig => {
      const formGroup = new FormGroup({});
      const templateGroup: TemplateSchema[] = [];

      groupConfig.formControlsData.forEach(controlData => {
        const control = new FormControl(controlData.value);
        this.addGuiValidators(control, controlData);
        this.addCustomValidators(control, controlData);

        const createdControlData: any = this.createControlData(controlData);
        templateGroup.push({inputs: {data: createdControlData, control: control}, component: controlData.component, type: TemplateGroupType.Control});
        formGroup.addControl(controlData.key, control);
      });
      if(!!groupConfig.nestedForm) {
        this.dynamicFormBuilder(formGroup, templateGroup, groupConfig.nestedForm);
      }
      form.addControl(groupConfig.groupName, formGroup);
      template.push({key: (groupConfig.groupName as string), type: TemplateGroupType.Group, children: templateGroup})
    });
  }


  private addGuiValidators(control: FormControl, controlData: FormControlData) {
    const guiValidators = this.createGuiValidators(controlData.guiValidators);
    guiValidators.length && control.addValidators(guiValidators)
  }

  private createGuiValidators(validators: GuiValidator[]): ValidatorFn[] {
    return validators.map((guiValidator: any) => {
      switch (guiValidator.type) {
        case GuiValidatorType.MaxLength:
          return Validators.maxLength(guiValidator.value);
        case GuiValidatorType.MinLength:
          return Validators.minLength(guiValidator.value);
        case GuiValidatorType.Required:
          return Validators.required;
        case GuiValidatorType.Email:
          return Validators.email;
        default:
          throw 'Gui validator not defined'
      }
    });
  }

  private addCustomValidators(control: FormControl, controlData: FormControlData) {
    const customSyncValidators = controlData.customValidators.filter(customValidator => customValidator.type === CustomValidatorType.Sync).map(syncValidator => syncValidator.validationFunction);
    !!customSyncValidators.length && control.addValidators((customSyncValidators as ValidatorFn[]));

    const customAsyncValidators = controlData.customValidators.filter(customValidator => customValidator.type === CustomValidatorType.Async).map(asyncValidator => asyncValidator.validationFunction);
    !!customAsyncValidators.length && control.addAsyncValidators((customAsyncValidators as AsyncValidatorFn[]));
  }

  private createControlData(controlData: FormControlData): any {
    switch (controlData.type) {
      case DynamicFormControlType.Checkbox:
      case DynamicFormControlType.Input:
        return {key: controlData.key, value: controlData.value, label: controlData.label};
      case DynamicFormControlType.Dropdown:
      case DynamicFormControlType.Radiobutton:
        return {key: controlData.key, value: controlData.value, label: controlData.label, options: controlData.dataSource};
      default:
        throw 'Gui validator not defined'
    }
  }
}

enum TemplateGroupType {
  Control,
  Group
}

export interface TemplateSchema {
  component?: any;
  children?: TemplateSchema[];
  type: TemplateGroupType;
  inputs: {data: any, control: FormControl}
  key?: string;
}
