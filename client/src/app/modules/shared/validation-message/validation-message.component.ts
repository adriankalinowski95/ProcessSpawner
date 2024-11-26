import {Component, EventEmitter, Input, Output} from '@angular/core';

@Component({
  selector: 'app-validation-message',
  standalone: true,
  templateUrl: './validation-message.component.html',
  styleUrl: './validation-message.component.scss'
})
export class ValidationMessageComponent {
  @Input() validationMessages: ValidationMessage[];
  @Output() closeValidationMessage: EventEmitter<void> = new EventEmitter<void>();

  protected readonly Object = Object;

  onCloseValidationMessage() {
    this.closeValidationMessage.emit();
  }
}
export type ValidationMessage = {
  [key: string]: [];
}
