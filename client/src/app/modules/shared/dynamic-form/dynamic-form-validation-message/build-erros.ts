import {TemplateSchema} from "../dynamic-form-content/dynamic-form-content.component";
import {ValidationMessage} from "../../validation-message/validation-message.component";

export const buildErrors = (templateSchema: TemplateSchema[], errorMessages: ValidationMessage[]) => {
  templateSchema.forEach(value => {
    if(value.children) {
      buildErrors(value.children, errorMessages)
    } else {
      const controlErrors = value.inputs.control.errors;
      const fieldName = value.inputs.data.key;
      if(controlErrors) {
        const errorKeys = Object.keys(controlErrors);
        const errors = new Array<any>();
        errorKeys.forEach(key => {
          const errorMessage = getErrorMessage(key, controlErrors[key]);
          errors.push(errorMessage);
        });
        let errorObj: any = {};
        errorObj[fieldName] = errors;
        errorMessages.push(errorObj);
      }
    }
  })
}

const getErrorMessage = (error: string, value?: any)=> {
  switch(error) {
    case 'required':
      return 'Field is required'
    case 'maxlength':
      return 'Value to long. Max length ' + value.requiredLength;
    case 'minlength':
      return 'Value to short. Min length ' + value.requiredLength;
    case 'email':
      return 'Wrong email pattern'
    default:
      return value;
  }
}
