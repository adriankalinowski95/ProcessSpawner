import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DynamicFormContentComponent } from './dynamic-form-content.component';

describe('DynamicFormContentComponent', () => {
  let component: DynamicFormContentComponent;
  let fixture: ComponentFixture<DynamicFormContentComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DynamicFormContentComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DynamicFormContentComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
