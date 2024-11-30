import { FormGroup } from "@angular/forms";
import { shared } from "../shared";


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

      var validationMessagesFieldMap = new Map(Object.entries(validationMessagesField));
      if (!shared.isNotNullOrUndefined(validationMessagesFieldMap)) {
        return defaultError;
      }

      if (!validationMessagesFieldMap.has(errorKey)) {
        return defaultError;
      }

      const finalMessage = validationMessagesFieldMap.get(errorKey) as string;

      return shared.isNotNullOrUndefined(finalMessage) ? finalMessage : defaultError;
    }

    return err;
  }
}