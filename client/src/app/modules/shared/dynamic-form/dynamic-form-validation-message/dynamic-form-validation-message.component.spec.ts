import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DynamicFormValidationMessageComponent } from './dynamic-form-validation-message.component';

describe('DynamicFormValidationMessageComponent', () => {
  let component: DynamicFormValidationMessageComponent;
  let fixture: ComponentFixture<DynamicFormValidationMessageComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DynamicFormValidationMessageComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DynamicFormValidationMessageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
