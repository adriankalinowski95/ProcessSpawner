import { Component } from '@angular/core';
import {ValidationMessage, ValidationMessageComponent} from "../../validation-message/validation-message.component";
import {TemplateSchema} from "../dynamic-form-content/dynamic-form-content.component";
import {ValidationErrors} from "@angular/forms";
import {buildErrors} from "./build-erros";

@Component({
  selector: 'app-dynamic-form-validation-message',
  templateUrl: './dynamic-form-validation-message.component.html',
  styleUrl: './dynamic-form-validation-message.component.scss'
})
export class DynamicFormValidationMessageComponent {

  validationErrors: [] = [];


  onCloseValidationMessage() {
    this.validationErrors = [];
  }

  createValidationMessage(templateSchema: TemplateSchema[]) {
    this.validationErrors = [];
    buildErrors(templateSchema, this.validationErrors);
  }
}
