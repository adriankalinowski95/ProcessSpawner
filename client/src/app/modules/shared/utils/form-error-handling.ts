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
          return false;
        }
    
        return control && control.errors != null && (control.touched || control.dirty);
    }
  
    getErrorKeys(fieldName: string): string[] {
        const control = this.m_form.get(fieldName);
        const result = control && control.errors ? Object.keys(control.errors) : [];
    
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
        const messages: { [key: string]: string } = {
          required: `${fieldName} jest wymagane.`,
          minlength: `${fieldName} musi mieć co najmniej ${errorValue.requiredLength} znaków.`,
          maxlength: `${fieldName} może mieć maksymalnie ${errorValue.requiredLength} znaków.`,
          pattern: `${fieldName} ma nieprawidłowy format.`,
        };
    
        return messages[errorKey] || `Nieznany błąd dla pola ${fieldName}.`;
      }
  }