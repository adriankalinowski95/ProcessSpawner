import {AfterViewInit, Component, EventEmitter, Input, Output, ViewChild} from '@angular/core';
import {
  ActionHandlerType,
  DynamicFormConfig, ErrorHandlerType
} from "./models/dynamic-form-config";
import {delay, Observable, Subject, tap} from "rxjs";
import {shared} from "../shared";
import ActionType = shared.action_button.models.ActionType;
import {DynamicFormContentComponent} from "./dynamic-form-content/dynamic-form-content.component";
import {
  DynamicFormValidationMessageComponent
} from "./dynamic-form-validation-message/dynamic-form-validation-message.component";
import {buildErrors} from "./dynamic-form-validation-message/build-erros";
import {ValidationMessage} from "../validation-message/validation-message.component";

@Component({
  selector: 'app-dynamic-form',
  templateUrl: './dynamic-form.component.html',
  styleUrl: './dynamic-form.component.scss'
})
export class DynamicFormComponent implements AfterViewInit{
  @Input() data: DynamicFormConfig
  @Output() submitData: EventEmitter<any> = new EventEmitter();
  @ViewChild(DynamicFormContentComponent) formContentComponent: DynamicFormContentComponent
  @ViewChild(DynamicFormValidationMessageComponent) validationMessageComponent: DynamicFormValidationMessageComponent

  outputData: any;
  actionHandlerType = ActionHandlerType;
  errorHandlerType = ErrorHandlerType;

  ngAfterViewInit() {
    this.formContentComponent.form.valueChanges.pipe(tap(value => {
        this.outputData = value;
    })).subscribe();
  }

  onButtonClick($event: ActionType) {
    const subscription = this.formContentComponent.form.statusChanges.pipe(
      tap(status => {
        if(status === 'PENDING') {
          return;
        }
        if(status === 'VALID') {
          this.submitData.emit(this.formContentComponent.form.value);
          subscription.unsubscribe();
        } else {
          this.validationMessageComponent.createValidationMessage(this.formContentComponent.templateSchema);
          this.submitData.emit(this.validationMessageComponent.validationErrors);
          subscription.unsubscribe();
        }
      })
    ).subscribe();
    this.formContentComponent.form.updateValueAndValidity();
    console.log(this.formContentComponent.form)
  }

  validateAsync(): Observable<boolean> {
    const validationMessage: ValidationMessage[] = [];
    const validationResult: Subject<boolean> = new Subject();
    const subscription = this.formContentComponent.form.statusChanges.pipe(
      delay(100),
      tap(status => {
        if(status === 'VALID') {
          validationResult.next(true);
          subscription.unsubscribe();
        }
        if(status === 'INVALID') {
          buildErrors(this.formContentComponent.templateSchema, validationMessage);
          validationResult.next(false);
          subscription.unsubscribe();
        }
      })
    ).subscribe();

    this.formContentComponent.form.updateValueAndValidity();

    return validationResult.pipe(tap(v => {
      if(!v) {
        throw JSON.stringify(validationMessage);
      }
    }));
  }
}
