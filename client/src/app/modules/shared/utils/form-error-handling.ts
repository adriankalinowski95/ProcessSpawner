import { FormGroup } from "@angular/forms";
import { shared } from "../shared";

/*
export class ErrorsHolder {
    public id: number = shared.getRandomNumber();
    public fieldName: string;
}
*/

export class ErrorKey {
    public readonly id: number = shared.getRandomNumber();
    public readonly key: string;

    constructor(data: { key: string }) {
        this.key = data.key;
    }
}

export class ErrorValidator {
    public readonly id: number = shared.getRandomNumber();
    public readonly key: string;
    public readonly message: string;

    constructor(data: { key: string, message: string }) {
        this.key = data.key;
        this.message = data.message;
    }
}

export class FormErrorHandlingService {
    private m_form: FormGroup;
    private m_validationMessages: any;
    
    constructor(form: FormGroup, validationMessages: any) {
        this.m_form = form;
        this.m_validationMessages = validationMessages;
    }
  
    controlHasErrors(fieldName: string): boolean {
        const control = this.m_form.get(fieldName);
        if (!control) {
            console.log("Control desont't exist");
          return false;
        }

        // const result = control && control.errors != null && (control.touched || control.dirty);
        return control.status == 'INVALID';
        
        /*
        console.log(control);
        console.log("result:" + result);

        return result;
        */
    }
  
    getErrorKeys(fieldName: string): string[] {
        const control = this.m_form.get(fieldName);
        const result = control && control.errors ? Object.keys(control.errors) : [];
        
        console.log("Get error keys");
        
        return result;
    }
  
    getErrorMessage(fieldName: string, errorKey: string): string {
      const control = this.m_form.get(fieldName);
      if (!control) {
        return '';
      }
      
      const defaultError = "Field " + fieldName + " had error: " + errorKey;
  
      const err = control.getError(errorKey);
  
      if (typeof err  == 'boolean') {
        var validationMessagesForFieldMap = new Map(Object.entries(this.m_validationMessages));
        if (!(validationMessagesForFieldMap.has(fieldName))) {
          return defaultError;
        }
  
        var validationMessagesField = validationMessagesForFieldMap.get(fieldName);
        if (!shared.isNotNullOrUndefined(validationMessagesField)) {
          return defaultError
        }
  
       // var validationMessagesFieldMap = new Map(Object.entries(validationMessagesField));
       var validationMessagesFieldMap = new Map(Object.entries(validationMessagesField));
        if (!shared.isNotNullOrUndefined(validationMessagesFieldMap)) {
          return defaultError;
        }

        let messageList = new Array<string>();
        validationMessagesFieldMap.forEach((value: any, key: any) => {
            messageList.push(value as string);
        });

        if (messageList.length == 0) {
          return defaultError;
        }

        const finalMessage = messageList.join("\n");
        //const finalMessage = validationMessagesFieldMap.get(errorKey) as string;
  
        return shared.isNotNullOrUndefined(finalMessage) ? finalMessage : defaultError;
      } else if (typeof err == 'object') {
        const message = this.getErrorMessageTranslation(fieldName, errorKey, err);

        return shared.isNotNullOrUndefined(message) ? message : defaultError;
      }
  
      return err;
    }

    getErrorMessageTranslation(fieldName: string, errorKey: string, errorValue: any): string {
        if (fieldName == null || fieldName == '') {
            return 'Unkwon error';
        }

        fieldName = fieldName[0].toUpperCase() + fieldName.slice(1);

        const messages: { [key: string]: string } = {
          required: `${fieldName} is required.`,
          minlength: `${fieldName} minimum length is ${errorValue.requiredLength}.`,
          maxlength: `${fieldName} maximum length is ${errorValue.requiredLength}.`,
          pattern: `${fieldName} have bad format.`,
        };
    
        return messages[errorKey] || `Unkown error of field ${fieldName}.`;
      }
  }